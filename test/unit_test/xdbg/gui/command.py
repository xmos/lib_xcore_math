
from xdbg.util import util

import numpy as np
import tkinter as tk
from tkinter import ttk


class CommandWindow(tk.Toplevel):

    def __init__(self, root, xgdb_ctrl, cmd):
        tk.Toplevel.__init__(self, master=root)

        self.ctrl = xgdb_ctrl
        self.cmd = cmd

        self.protocol("WM_DELETE_WINDOW", self.on_kill)
        self._layout()

        self.ctrl.add_refresh_cb(self.on_refresh)
        self.on_refresh(self.ctrl)

    def deinit(self):
        self.ctrl.remove_refresh_cb(self.on_refresh)

    def on_kill(self):
        self.deinit()
        self.destroy()

    def on_refresh(self, ctrl, *args):
        resp = self.ctrl.send_command(self.cmd, clear_replay=True)
        self.text.delete(1.0,tk.END)
        self.text.insert(tk.END, resp)

    def _layout(self):
        self.title(f"Cmd: {self.cmd}")
        self.attributes('-topmost', True)

        self.text = tk.Text(self)
        self.text.pack()


