# Recovery artifacts

lk-volup-fastboot-r1.img: signed LK candidate adding a boot-time Volume UP fastboot branch. 28-byte code patch plus updated LK cert2 payload hash/signature; DT unchanged. Signature/hash checks and 16 ARM emulation scenarios pass. NOT hardware-validated. See ../docs/20-lk-key-entry-r1.md and lk-volup-fastboot-r1.json before device testing.

lk-keys-fastboot-r2.img: current signed LK with user-confirmed boot-time key entry success. r1 cold-boot keys did not work after successful lk writes. r2 polls HW keys 0/1/17/19 for about 3 seconds, feeds the watchdog, and adds an S17-LK-r2 marker to the Orange State countdown line. Signature/hash checks and 44 ARM scenarios pass. Individual keys and behavior after Lineage flashing have not been separately reported. See ../docs/21-lk-key-entry-r2.md.

recovery-384.img: device-verified boot/display/root ADB. Runtime chcon confirmed fastbootd can then recognize system.
recovery-r4.img: current candidate; same files as recovery-384 except init RC automatically performs that chcon on boot. Build/static/init verification pass; user confirms it boots, but automatic super relabeling did not take effect.
recovery-r3.img: DO NOT USE. User confirmed boot failure; preserved for audit.
recovery-r2.img: adds confirmation resources; not separately boot-tested.
recovery.img: original historical build.

See ../docs/15-super-label-confirmed.md and SHA256SUMS. Boot regression of r3 remains unexplained; r4 does not claim to diagnose it.

recovery-r5.img: current device-validated recovery. Adds a named coldboot-aware relabel service with /tmp/s17-super-label.log and vendor.recovery.super_label.status; reapplies before fastbootd. Build/init/static checks pass. User confirms ready status, successful automatic chcon and fastbootd logical system recognition. r4 boots but automatic label did not take effect.
