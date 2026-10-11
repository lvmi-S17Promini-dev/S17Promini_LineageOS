#!/system/bin/sh
# One read-only snapshot. Run before screen-off and after waking manually.
set -u
out=${1:-/data/local/tmp/s17-power-debug}
if [ "$(id -u)" != 0 ]; then
    echo 'Run in a root phone shell.' >&2
    exit 1
fi
if [ -e "$out" ]; then
    echo "Choose a new output directory: $out already exists." >&2
    exit 1
fi
mkdir -p "$out" || exit 1
read_node() {
    if [ -f "$1" ]; then
        printf '\n### %s\n' "$1"
        timeout 5 cat "$1" 2>&1
    fi
}
{
    uname -a
    read_node /proc/cmdline
    read_node /proc/uptime
    read_node /sys/devices/system/cpu/online
    read_node /sys/power/state
    read_node /sys/power/mem_sleep
    read_node /sys/power/wakeup_count
    read_node /sys/kernel/debug/suspend_stats
    for f in /sys/power/suspend_stats/*; do read_node "$f"; done
} >"$out/system.txt" 2>&1
{
    for p in /sys/devices/system/cpu/cpufreq/policy*; do
        for name in affected_cpus related_cpus scaling_driver scaling_governor \
            scaling_available_governors scaling_available_frequencies \
            scaling_min_freq scaling_max_freq cpuinfo_min_freq cpuinfo_max_freq \
            scaling_cur_freq cpuinfo_cur_freq stats/time_in_state; do
            read_node "$p/$name"
        done
    done
    read_node /proc/cpufreq/cpufreq_sched_disable
    read_node /proc/cpufreq/cpufreq_power_mode
    for p in /proc/cpufreq/MT_CPU_DVFS_*; do
        read_node "$p/cpufreq_oppidx"
        read_node "$p/cpufreq_freq"
    done
    read_node /proc/ppm/enabled
    read_node /proc/ppm/policy_status
    read_node /proc/ppm/dump_policy_list
} >"$out/cpufreq-ppm.txt" 2>&1
{
    read_node /proc/mcdi/state
    read_node /proc/mcdi/info
    for name in idle_state dpidle_state soidle_state soidle3_state; do
        read_node "/proc/mtk_lpm/cpuidle/$name"
        read_node "/sys/kernel/debug/mtk_lpm/cpuidle/$name"
    done
    for s in /sys/devices/system/cpu/cpu[0-9]*/cpuidle/state*; do
        for name in name desc disable usage time; do read_node "$s/$name"; done
    done
} >"$out/idle.txt" 2>&1
{
    read_node /sys/kernel/debug/wakeup_sources
    read_node /sys/kernel/wakeup_reasons/last_resume_reason
    read_node /sys/kernel/wakeup_reasons/last_suspend_time
    for w in /sys/class/wakeup/wakeup*; do
        for name in name active_count event_count wakeup_count active_time_ms \
            total_time_ms prevent_suspend_time_ms; do read_node "$w/$name"; done
    done
} >"$out/wakeup.txt" 2>&1
timeout 15 dumpsys power >"$out/dumpsys-power.txt" 2>&1
timeout 15 dumpsys suspend_control >"$out/dumpsys-suspend.txt" 2>&1
timeout 15 dumpsys suspend_control_internal >"$out/dumpsys-suspend-internal.txt" 2>&1
timeout 15 dumpsys cpuinfo >"$out/dumpsys-cpuinfo.txt" 2>&1
timeout 15 dumpsys battery >"$out/dumpsys-battery.txt" 2>&1
timeout 15 dumpsys batterystats >"$out/dumpsys-batterystats.txt" 2>&1
cat /proc/stat >"$out/proc-stat.txt"
cat /proc/interrupts >"$out/interrupts.txt"
timeout 10 top -b -n 1 >"$out/top.txt" 2>&1
dmesg >"$out/dmesg.txt" 2>&1
timeout 15 logcat -b all -d -t 4000 >"$out/logcat.txt" 2>&1
echo "Saved $out"
