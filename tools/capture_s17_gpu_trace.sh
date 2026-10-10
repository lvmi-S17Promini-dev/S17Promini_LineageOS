#!/system/bin/sh
# Run as root on the phone. No kernel rebuild, power-policy change or fence writes.
set -u

out=${1:-/data/local/tmp/s17-gpu-trace}
probe=${2:-}
case "$probe" in
    ''|--restart-bootanim) ;;
    *) echo 'Usage: sh capture_s17_gpu_trace.sh OUTPUT [--restart-bootanim]' >&2; exit 1 ;;
esac
if [ "$(id -u)" != 0 ]; then
    echo 'Run this script in a root adb shell.' >&2
    exit 1
fi
if [ -e "$out" ]; then
    echo "Output already exists: $out; choose a new directory." >&2
    exit 1
fi
mkdir -p "$out" || exit 1
exec >"$out/capture.txt" 2>&1
date
uname -a

root=
for candidate in /proc/pvr /proc/pvrsrvkm /sys/kernel/debug/pvr /sys/kernel/debug/pvrsrvkm; do
    if [ -f "$candidate/apphint/0/EnableLogGroup" ]; then
        root=$candidate
        break
    fi
done
if [ -z "$root" ]; then
    echo 'GPU AppHint nodes unavailable; no settings changed.'
    echo 'If debugfs is not mounted, mount it and rerun with a new output directory.'
    ls -ld /proc/pvr /proc/pvrsrvkm /sys/kernel/debug
    cat /proc/mounts
    exit 2
fi
echo "GPU diagnostic root: $root"
hint=$root/apphint/0
timeout 10 cat "$hint/EnableLogGroup" >"$out/log-group-before.txt" || exit 3
timeout 10 cat "$hint/FirmwareLogType" >"$out/log-type-before.txt" || exit 3
old_group=$(cat "$out/log-group-before.txt")
old_type=$(cat "$out/log-type-before.txt")
old_bootanim_exit=
bootanim_changed=0
trace_sampler=

write_hint() {
    # Some Android shell printf implementations write text and newline
    # separately. AppHint parses each write as a complete value, so the
    # second write is rejected. Feed the complete short line through cat.
    printf '%s\n' "$2" >"$out/hint-value.txt" || return 1
    timeout 10 sh -c 'cat "$2" > "$1"' sh "$1" "$out/hint-value.txt"
}
restore() {
    if [ -n "$trace_sampler" ]; then
        kill "$trace_sampler" 2>/dev/null
        wait "$trace_sampler" 2>/dev/null
    fi
    if [ "$bootanim_changed" = 1 ]; then
        setprop service.bootanim.exit "$old_bootanim_exit"
    fi
    # The write handlers are asynchronous: record the eventual readback too.
    case "$old_type" in
        trace|tbi) write_hint "$hint/FirmwareLogType" "$old_type" ;;
        *) echo 'Original log mode had no printable value; restoring the group filter.' ;;
    esac
    write_hint "$hint/EnableLogGroup" "$old_group"
    sleep 1
    timeout 10 cat "$hint/EnableLogGroup" >"$out/log-group-restored.txt"
    timeout 10 cat "$hint/FirmwareLogType" >"$out/log-type-restored.txt"
}
trap restore EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

timeout 10 cat "$root/firmware_trace" >"$out/fw-before.txt"
timeout 10 cat "$root/debug_dump" >"$out/gpu-before.txt"
write_hint "$hint/FirmwareLogType" trace || exit 4
write_hint "$hint/EnableLogGroup" main,mts,bif,pm,rtd,spm,pow,hwr || exit 4
sleep 1
timeout 10 cat "$hint/FirmwareLogType" >"$out/log-type-enabled.txt"
timeout 10 cat "$hint/EnableLogGroup" >"$out/log-group-enabled.txt"
if ! grep -q '^trace$' "$out/log-type-enabled.txt" ||
   ! grep -q 'main' "$out/log-group-enabled.txt"; then
    echo 'Firmware trace settings did not take effect; see readback and dmesg.'
    timeout 10 dmesg >"$out/dmesg.txt"
    exit 5
fi
if [ "$probe" = --restart-bootanim ]; then
    # The firmware ring is small and MTS/power traffic can overwrite the
    # first KICK before the final ten-second snapshot. Preserve snapshots
    # throughout restart, not just at the end of the capture window.
    mkdir "$out/fw-samples" || exit 6
    (
        sample=0
        while [ "$sample" -lt 100 ]; do
            name=$(printf '%03d' "$sample")
            cat /proc/uptime >"$out/fw-samples/$name.time.txt"
            timeout 2 cat "$root/firmware_trace" >"$out/fw-samples/$name.trace.txt"
            sample=$((sample + 1))
            sleep 0.1
        done
    ) &
    trace_sampler=$!
    # Capture fresh render submissions instead of only retries/cleanup of
    # contexts which stalled before tracing was enabled.
    old_bootanim_exit=$(getprop service.bootanim.exit)
    getprop init.svc.bootanim >"$out/bootanim-state-before.txt"
    printf '%s\n' "$old_bootanim_exit" >"$out/bootanim-exit-before.txt"
    bootanim_changed=1
    setprop service.bootanim.exit 0 || exit 6
    setprop ctl.restart bootanim || exit 6
    echo 'Requested bootanim restart with firmware tracing already enabled.'
fi
# Wake the display and allow already-submitted render work to be scheduled.
timeout 10 input keyevent 224
sleep 10
if [ -n "$trace_sampler" ]; then
    wait "$trace_sampler"
    trace_sampler=
fi
getprop init.svc.bootanim >"$out/bootanim-state-after.txt"
pidof bootanimation >"$out/bootanim-pid-after.txt"
timeout 10 cat "$root/firmware_trace" >"$out/fw-after.txt"
timeout 10 cat "$root/debug_dump" >"$out/gpu-after.txt"
timeout 10 cat /proc/gpulog >"$out/gpu-proc.txt"
timeout 10 dmesg >"$out/dmesg.txt"
timeout 15 logcat -b all -d -v threadtime >"$out/logcat.txt"
echo "Capture complete: $out"
