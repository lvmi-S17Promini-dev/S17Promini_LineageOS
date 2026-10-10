#!/system/bin/sh
# Manual PRAM trial, read probe or explicit guarded flash repair.
# Only --repair programs flash; no mode invokes chip erase.
set -u
out=${1:-/data/local/tmp/s17-pram-test}
fw_dir=${2:-/data/local/tmp}
mode=${3:-}
case "$mode" in
    ''|--probe|--repair) ;;
    *) echo 'Third argument must be --probe, --repair or omitted.' >&2; exit 1 ;;
esac
node=/sys/bus/i2c/devices/0-003b
path_node=/sys/module/firmware_class/parameters/path
if [ "$mode" = --probe ] && [ ! -f "$node/firmware_probe" ]; then
    echo 'This boot does not include the read-probe interface.' >&2
    exit 1
fi
if [ "$mode" = --repair ] && [ ! -f "$node/firmware_repair" ]; then
    echo 'This boot does not include the guarded FF repair interface.' >&2
    exit 1
fi
if [ "$(id -u)" != 0 ] || [ ! -f "$node/firmware_pram" ]; then
    echo 'Need a root shell and a boot with the PRAM test interface.' >&2
    exit 1
fi
if [ ! -f "$fw_dir/axs15260-s17.bin" ] || [ ! -w "$path_node" ]; then
    echo 'Missing axs15260-s17.bin or writable firmware_class path parameter.' >&2
    exit 1
fi
if [ -e "$out" ]; then
    echo "Choose a new output directory: $out already exists." >&2
    exit 1
fi
if [ "$(cat "$node/debug_poll_ms")" != 0 ]; then
    echo 'Stop diagnostic polling before the PRAM trial.' >&2
    exit 1
fi
mkdir -p "$out" || exit 1
old_path=$(cat "$path_node")
write_path() {
    # String module parameters preserve newline. A NUL-terminated short file
    # writes an exact path and also permits restoring an originally empty path.
    printf '%s\0' "$1" >"$out/path-value.bin"
    cat "$out/path-value.bin" >"$path_node"
}
trap 'write_path "$old_path"' EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

date >"$out/time.txt"
timeout 10 input keyevent 224
timeout 10 cat "$node/diagnostics" >"$out/before.status.txt" 2>&1
timeout 10 cat "$node/live_version" >"$out/before.version.txt" 2>&1
write_path "$fw_dir" || exit 2
cat "$path_node" >"$out/firmware-path.txt"
if [ "$mode" = --repair ]; then
    echo 'Preparing a complete readout before any programming...'
    timeout 20 sh -c 'echo 1 > "$1"' sh "$node/firmware_probe" >"$out/prepare.txt" 2>&1
    ret=$?
    if [ "$ret" != 0 ]; then
        cat "$node/firmware_status" >"$out/firmware-status.txt"
        echo 'Read preparation failed; repair was not invoked.' >&2
        exit "$ret"
    fi
    cat "$node/flash_dump" >"$out/before.bin" || exit 3
    cat "$node/flash_status" >"$out/before.flash-status.txt"
    cat "$node/flash_regions" >"$out/before.regions.txt"
    echo 'Explicit FF repair: program missing stock bytes and verify; no erase.'
    timeout 40 sh -c 'echo repair-stock-0007 > "$1"' sh "$node/firmware_repair" >"$out/load.txt" 2>&1
elif [ "$mode" = --probe ]; then
    echo 'Reading identity, status and stored firmware through the stock flash interface...'
    timeout 20 sh -c 'echo 1 > "$1"' sh "$node/firmware_probe" >"$out/load.txt" 2>&1
else
    echo 'Loading the stock firmware through the manual PRAM path...'
    timeout 20 sh -c 'echo load > "$1"' sh "$node/firmware_pram" >"$out/load.txt" 2>&1
fi
ret=$?
echo "$ret" >"$out/load-exit.txt"
cat "$node/firmware_status" >"$out/firmware-status.txt"
if [ "$mode" = --probe ] || [ "$mode" = --repair ]; then
    cat "$node/flash_status" >"$out/flash-status.txt"
    if [ -f "$node/flash_regions" ]; then
        cat "$node/flash_regions" >"$out/regions.txt"
        cat "$node/flash_dump" >"$out/readout.bin" 2>"$out/readout-error.txt"
    fi
    if [ "$mode" = --repair ]; then
        cat "$node/flash_backup" >"$out/backup.bin" 2>"$out/backup-error.txt"
    fi
fi
cat "$node/diagnostics" >"$out/after.status.txt"
timeout 10 cat "$node/live_version" >"$out/after.version.txt" 2>&1
event=
for name in /sys/class/input/event*/device/name; do
    if [ "$(cat "$name")" = axs_ts ]; then
        event=/dev/input/$(basename "$(dirname "$(dirname "$name")")")
        break
    fi
done
if [ "$ret" = 0 ] && [ -n "$event" ]; then
    if [ "$mode" = --repair ] || [ "$mode" = --probe ]; then
        timeout 10 input keyevent 224
    fi
    echo 'Now touch/swipe the screen for eight seconds; avoid screen-off/reset.'
    timeout 8 getevent -lt "$event" >"$out/touch-events.txt" 2>&1
fi
cat /proc/interrupts >"$out/irq-counts.txt"
cat "$node/diagnostics" >"$out/final.status.txt"
cat "$node/report_history" >"$out/history.txt"
timeout 10 cat "$node/live_version" >"$out/final.version.txt" 2>&1
timeout 10 dmesg >"$out/dmesg.txt" 2>&1
echo "Saved: $out (loader exit $ret)"
exit "$ret"
