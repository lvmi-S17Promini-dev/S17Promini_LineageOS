#!/usr/bin/env python3
import os
import sys
import logging
import time

sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from mtkclient.Library.mtk_main import Mtk
from mtkclient.config.mtk_config import MtkConfig
from mtkclient.Library.mtk_preloader import Preloader

def run_exploit():
    valid_modes = ["fastboot", "bybutton", "brom", "meta"]
    chosen_mode = None

    if len(sys.argv) > 1:
        arg_mode = sys.argv[1].strip().lower()
        if arg_mode in valid_modes:
            chosen_mode = arg_mode

    if not chosen_mode:
        print("\n--- MTK REDIRECTION TOOL ---")
        print("Available modes:")
        print("  1. fastboot : Boot straight into Fastboot mode")
        print("  2. bybutton : Release flag (Choose mode by pressing volume down or up)")
        print("  3. brom     : Force deep BROM fallback hardware state")
        print("  4. meta     : Boot into META diagnostics environment")
        print("----------------------------")
        
        while True:
            user_input = input("Enter redirection mode: ").strip().lower()
            if user_input in valid_modes:
                chosen_mode = user_input
                break
            print(f"[-] Invalid selection. Choose from: {', '.join(valid_modes)}")

    print(f"\n[*] Mode Selected: {chosen_mode.upper()}")

    # Countdown sequence prior to USB socket connection
    if chosen_mode == "bybutton":
        print("\n=======================================================")
        print("==> Choose mode by pressing volume down or up NOW! <==")
        print("=======================================================")
        for i in range(3, 0, -1):
            print(f"Keep holding keys! Connecting in {i}...")
            time.sleep(1)
        print("\n[*] NOW plug in the USB cable while holding the keys...")
    else:
        print("\n[*] Power off the phone and connect the USB cable...")

    print("[*] Initializing mtkclient configurations...")
    config = MtkConfig(loglevel=logging.INFO, gui=None, guiprogress=None)
    mtk = Mtk(config=config, loglevel=logging.INFO)
    
    if mtk.preloader.init():
        print("\n[+] Connected successfully!")
        
        wdt_base = None
        
        # Check newer mtkclient layout structure
        if hasattr(mtk.config, 'chipconfig') and mtk.config.chipconfig.watchdog is not None:
            wdt_base = mtk.config.chipconfig.watchdog
        # Check older mtkclient layout structure
        elif hasattr(mtk.config, 'watchdog') and mtk.config.watchdog is not None:
            wdt_base = mtk.config.watchdog

        # If both checks return nothing, abort execution safely instead of guessing
        if wdt_base is None:
            print("[-] Error: Could not automatically detect the Watchdog (WDT) address for this device.")
            print("[*] Please ensure your mtkclient version is 2.1.4 or up to date.")
            return

        print(f"[!] Target Watchdog Base Detected Automatically: {hex(wdt_base)}")
        
        pl = Preloader(mtk, loglevel=logging.INFO)
        print(f"[!] Sending {chosen_mode.upper()} payload instantly...")

        if chosen_mode == "fastboot":
            pl.direct_fastboot(wdt_base)
        elif chosen_mode == "bybutton":
            pl.direct_factory(wdt_base)
        elif chosen_mode == "brom":
            pl.direct_brom(wdt_base)
        elif chosen_mode == "meta":
            pl.direct_meta(wdt_base)

        time.sleep(1)
        print("[+] Operation Complete. Disconnect cable once targeted layer initializes.")
    else:
        print("[-] Error: Failed to catch the Preloader/Brom connection interface.")

if __name__ == "__main__":
    run_exploit()
