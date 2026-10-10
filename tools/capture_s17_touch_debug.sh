#!/system/bin/sh
# Run in a root phone shell after pushing this script. Touch/swipe during capture.
set -u
out=${1:-/data/local/tmp/s17-touch-debug}
node=/sys/bus/i2c/devices/0-003b
if [ "$(id -u)" != 0 ]; then
    echo 'Run in a root shell.' >&2
    exit 1
fi
if [ ! -f "$node/debug_poll_ms" ]; then
    echo 'This boot does not have the extended touch diagnostics.' >&2
    exit 1
fi
if [ -e "$out" ]; then
    echo "Choose a new output directory: $out already exists." >&2
    exit 1
fi
old_poll=$(cat "$node/debug_poll_ms")
if [ "$old_poll" != 0 ]; then
    echo 'Stop the existing diagnostic polling before running this capture.' >&2
    exit 1
fi
mkdir -p "$out" || exit 1
old_reports=$(cat "$node/debug_reports")
i2c_trace_node=
old_i2c_trace=
for candidate in /sys/module/*/parameters/s17_touch_debug; do
    if [ -f "$candidate" ]; then
        i2c_trace_node=$candidate
        old_i2c_trace=$(cat "$candidate")
        break
    fi
done
cleanup() {
    echo 0 >"$node/debug_poll_ms"
    echo "$old_reports" >"$node/debug_reports"
    if [ -n "$i2c_trace_node" ]; then
        echo "$old_i2c_trace" >"$i2c_trace_node"
    fi
}
trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

snapshot() {
    tag=$1
    cat /proc/uptime >"$out/$tag.time.txt"
    timeout 10 cat "$node/diagnostics" >"$out/$tag.status.txt" 2>&1
    timeout 10 cat "$node/report_history" >"$out/$tag.history.txt" 2>&1
    timeout 10 cat /sys/devices/platform/pinctrl@1000b000/mt_gpio >"$out/$tag.gpio.txt" 2>&1
    timeout 10 cat /proc/interrupts >"$out/$tag.irq-counts.txt"
}

echo 'Wake screen; touch and swipe for the next 15 seconds.'
timeout 10 input keyevent 224
snapshot before
if [ -n "$i2c_trace_node" ]; then
    echo 1 >"$i2c_trace_node"
    cat "$i2c_trace_node" >"$out/i2c-trace-enabled.txt"
fi
timeout 10 cat "$node/live_version" >"$out/live-version.txt" 2>&1
timeout 10 cat "$node/supply_state" >"$out/supply-state.txt" 2>&1
timeout 10 cat /sys/kernel/debug/irq/irqs/16 >"$out/irq-state.txt" 2>&1
timeout 10 cat /sys/kernel/debug/regulator/regulator_summary >"$out/regulators.txt" 2>&1
echo 1 >"$node/debug_reports" || exit 2
echo 20 >"$node/debug_poll_ms" || exit 2
i=0
while [ "$i" -lt 15 ]; do
    sleep 1
    tag=$(printf '%02d' "$i")
    snapshot "$tag"
    i=$((i + 1))
done
echo 0 >"$node/debug_poll_ms"
snapshot after
timeout 10 dmesg >"$out/dmesg.txt" 2>&1
timeout 15 logcat -b all -d -v threadtime >"$out/logcat.txt" 2>&1
echo "Saved: $out"
