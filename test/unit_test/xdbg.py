
import argparse
import subprocess
import os
import signal
import atexit
import threading
import sys
import tkinter as tk
import time

import xdbg as xw
from xdbg.util import util

XGDB_EXECUTABLE = "xgdb"

xgdb_proc = None


def start_daemon(func, *args):
    t = threading.Thread(target=func)
    t.daemon = True
    t.start()
    return t


def kill_xgdb():
    if (xgdb_proc is not None) and (xgdb_proc.poll() is None):
        os.kill(xgdb_proc.pid, signal.SIGTERM)

def force_quit(signum, stack):
    sys.exit()



def main(args):
    global xgdb_proc

    cmd = [XGDB_EXECUTABLE, args.binary]

    if args.func:
        cmd.append(f'--eval-command=break {args.func}')
    if args.connect:
        cmd.append("--eval-command=connect")
    if args.run:
        cmd.append("--eval-command=run")

    root = tk.Tk()
    root.overrideredirect(1)

    xgdb_proc = subprocess.Popen(cmd, bufsize=0, 
                                 stdout=subprocess.PIPE, stdin=subprocess.PIPE,
                                 stderr=subprocess.PIPE, text=True)

    # Main point of contact with XGDB
    xgdb_ctrl = xw.XgdbController(xgdb_proc)

    xgdb_ctrl.meta_commands["test"] = lambda: util.get.vpu_state(xgdb_ctrl)

    # Handles UI window commands
    wins = xw.WindowMgr(root, xgdb_ctrl)

    # Terminate this process if xgdb terminates.
    def wait_and_exit():
        xgdb_proc.wait()
        root.destroy()
    start_daemon(wait_and_exit)

    # Wait for a prompt to show up
    xgdb_ctrl.wait_for_prompt()

    def wait_and_withdraw():
        # time.sleep()
        root.withdraw()

    # start_daemon(wait_and_withdraw)

    root.after(1, root.withdraw)

    # Blocks until root is destroyed
    root.mainloop()


if (__name__ == "__main__"):
    parser = argparse.ArgumentParser()

    parser.add_argument("binary")
    parser.add_argument("func", nargs='?', default=None)
    parser.add_argument("-c", "--connect", action="store_true", default=False)
    parser.add_argument("-r", "--run", action="store_true", default="False")

    # Flags to show some windows at start-up?

    args = parser.parse_args()

    signal.signal(signal.SIGINT, force_quit)

    main(args)




atexit.register(kill_xgdb)