#!/usr/bin/env python3
"""Save pre-reboot ADB streams on the host, or collect recovery evidence.

Does not initiate reboot, clear logs, or format partitions. Kernel shutdown
after adbd stops must be recovered from pstore, not from these live streams.
"""
import argparse
from datetime import datetime
import json
import os
from pathlib import Path
import shlex
import subprocess
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("mode", choices=("capture", "recovery"))
    parser.add_argument("--serial", help="ADB device serial")
    parser.add_argument("--out", type=Path, help="New host output directory")
    parser.add_argument("--seconds", type=int, default=180,
                        help="Maximum live capture time; default 180 seconds")
    args = parser.parse_args()
    if args.seconds < 1:
        parser.error("--seconds must be positive")
    adb = ["adb"] + (["-s", args.serial] if args.serial else [])
    out = args.out or Path("oss-kernel-debug") / (
        "reboot-" + args.mode + "-" + datetime.now().strftime("%Y%m%d-%H%M%S-%f"))

    def command(argv, timeout=15, check=True):
        return subprocess.run(adb + argv, stdout=subprocess.PIPE,
                              stderr=subprocess.PIPE, timeout=timeout, check=check)

    # Do not restart adbd here: the user may already have a working root shell.
    uid = command(["shell", "id -u"]).stdout.strip()
    prefix = ""
    if uid != b"0":
        uid = command(["shell", "su 0 id -u"]).stdout.strip()
        if uid != b"0":
            raise RuntimeError("Root required. Run adb root first, then retry.")
        prefix = "su 0 "

    def shell(script):
        return prefix + "sh -c " + shlex.quote(script)

    out.mkdir(parents=True, exist_ok=False)
    boot_id = command(["shell", shell("cat /proc/sys/kernel/random/boot_id")]).stdout.decode().strip()
    metadata = {"mode": args.mode, "boot_id": boot_id,
                "started": datetime.now().isoformat(), "adb": adb,
                "output": str(out.resolve())}
    (out / "capture.json").write_text(json.dumps(metadata, indent=2) + "\n")

    def save(name, script, timeout=20):
        with (out / name).open("wb") as stdout, (out / (name + ".stderr")).open("wb") as stderr:
            try:
                result = subprocess.run(adb + ["exec-out", shell(script)],
                                        stdout=stdout, stderr=stderr, timeout=timeout)
                metadata[name] = {"returncode": result.returncode}
            except subprocess.TimeoutExpired:
                metadata[name] = {"error": "timeout; partial output retained"}

    save("identity.txt", "uname -a; cat /proc/cmdline; cat /proc/uptime; cat /proc/mounts")
    save("pstore-before.tar" if args.mode == "capture" else "pstore.tar",
         "if ! grep -q ' /sys/fs/pstore pstore ' /proc/mounts; then "
         "mkdir -p /sys/fs/pstore; mount -t pstore pstore /sys/fs/pstore || exit; fi; "
         "tar -C /sys/fs/pstore -cf - .")
    if args.mode == "recovery":
        # Mount only the existing cache filesystem; never wipe or delete files.
        save("cache-mount.txt", "grep ' /cache ' /proc/mounts || mount /cache")
        save("cache-logs.tar", "cd /cache || exit; "
             "set --; for p in s17-early-kmsg.log s17-bootlog s17-reboot; do "
             "if [ -e \"$p\" ]; then set -- \"$@\" \"$p\"; fi; done; "
             "if [ \"$#\" -eq 0 ]; then echo 'No cache capture files' >&2; exit 1; fi; "
             "tar -cf - \"$@\"", timeout=45)
        save("recovery.log", "cat /tmp/recovery.log")
        save("recovery-dmesg.txt", "dmesg")
    else:
        marker = "[S17/REBOOT] capture armed boot_id=" + boot_id
        save("arm.txt", "printf '%s\\n' " + shlex.quote("<6>" + marker) + " > /dev/kmsg")
        if metadata["arm.txt"].get("returncode") != 0:
            raise RuntimeError("Cannot write the kmsg marker; check arm.txt.stderr")
        save("dmesg-before.txt", "dmesg")
        save("logcat-before.txt", "logcat -b all -d -v threadtime")
        save("state-before.txt", "ps -A; cat /sys/devices/system/cpu/online; "
             "for p in /sys/devices/system/cpu/cpufreq/policy*; do "
             "echo \"$p\"; cat \"$p/scaling_governor\" \"$p/scaling_cur_freq\"; done")
        streams = []
        try:
            for name, script in (("kmsg-live.txt", "dmesg -w"),
                                 ("logcat-live.txt", "logcat -b all -v threadtime")):
                stdout = (out / name).open("wb", buffering=0)
                stderr = (out / (name + ".stderr")).open("wb", buffering=0)
                try:
                    process = subprocess.Popen(adb + ["exec-out", shell(script)],
                                               stdout=stdout, stderr=stderr)
                except BaseException:
                    stdout.close()
                    stderr.close()
                    raise
                streams.append((name, process, stdout, stderr))
            print(f"Recording to {out.resolve()}. Reboot from the phone's menu now.", flush=True)
            print("ADB ending during shutdown is expected. Then enter recovery directly.", flush=True)
            end = time.monotonic() + args.seconds
            while time.monotonic() < end:
                time.sleep(0.5)
                for _, _, stdout, _ in streams:
                    os.fsync(stdout.fileno())
                if all(process.poll() is not None for _, process, _, _ in streams):
                    break
        except KeyboardInterrupt:
            print("Capture stopped; saved output retained.", flush=True)
        finally:
            for name, process, stdout, stderr in streams:
                if process.poll() is None:
                    process.terminate()
                    try:
                        process.wait(timeout=3)
                    except subprocess.TimeoutExpired:
                        process.kill()
                        process.wait()
                metadata[name] = {"returncode": process.returncode}
                os.fsync(stdout.fileno())
                stdout.close()
                stderr.close()
    (out / "capture.json").write_text(json.dumps(metadata, indent=2) + "\n")
    failed = [name for name, result in metadata.items() if isinstance(result, dict)
              and (result.get("error") or result.get("returncode", 0) != 0)]
    if failed:
        print("Partial capture; inspect stderr and capture.json for: " + ", ".join(failed))
    print(f"Saved: {out.resolve()}")


if __name__ == "__main__":
    try:
        main()
    except (OSError, RuntimeError, subprocess.SubprocessError) as error:
        raise SystemExit(str(error))
