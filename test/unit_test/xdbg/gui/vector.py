
from xdbg.util import util

import numpy as np
import tkinter as tk
from tkinter import ttk


class VpuWindow(tk.Toplevel):

    def __init__(self, root, xgdb_ctrl):
        tk.Toplevel.__init__(self, master=root)

        self._layout()

        self.ctrl = xgdb_ctrl


        self.ctrl.add_refresh_cb(self.on_refresh)
        self.on_refresh(self.ctrl)

    def deinit(self):
        self.ctrl.remove_refresh_cb(self.on_refresh)
        self.destroy()

    def on_refresh(self, ctrl, *args):
        state = util.get.vpu_state(ctrl)
        
        self.txt_vsr.set("0x%04X" % state.vsr)

        for mode,elms in [(32,8),(16,16),(8,32)]:
            for i in range(elms):
                for vv,vec in [(self.vR, state.vR),(self.vD, state.vD),(self.vC, state.vC)]:
                    if   mode == 32: vv[mode][i].set("0x%08X" % vec.uint32[i])
                    elif mode == 16: vv[mode][i].set("0x%04X" % vec.uint16[i])
                    elif mode ==  8: vv[mode][i].set("0x%02X" % vec.uint8[i] )

        acc40 = state.acc40
        acc32 = state.acc32

        for i in range(8):
            self.acc[32][i].set("0x%010X" % acc40[i])
        for i in range(16):
            self.acc[16][i].set("0x%08X" % acc32[i])

    def _layout(self):

        self.title("VPU")
        self.attributes('-topmost', True)
        self.resizable(False, False)
        # self.overrideredirect(1)

        self.txt_vsr = tk.StringVar(); self.txt_vsr.set("--")

        tk.Entry(self, width=8, state="readonly", textvariable=self.txt_vsr).grid(row=0, column=1, sticky=tk.W)

        tk.Label(self, width=5, text="VSR").grid(row=0)

        nb = ttk.Notebook(self)
        nb.grid(row=1,rowspan=4,columnspan=5,sticky=tk.N+tk.S+tk.E+tk.W)

        tab32 = ttk.Frame(nb)
        tab16 = ttk.Frame(nb)
        tab8 = ttk.Frame(nb)

        nb.add(tab32, text="32-Bit")
        nb.add(tab16, text="16-Bit")
        nb.add(tab8,  text="8-Bit")

        self.vR = {}
        self.vD = {}
        self.vC = {}
        self.acc = {}

        for mode,elms,acc_size,tab in [(32,8,40,tab32),(16,16,32,tab16),(8,32,None,tab8)]:

            for d,reg in enumerate(["vR", "vD", "vC"]):
                tk.Label(tab, text=reg).grid(row=d+1, column=0)

            for i in range(elms):
                tk.Label(tab, text=str(i)).grid(row=0,column=i+1)

            for d,reg in enumerate([self.vR, self.vD, self.vC]):
                reg[mode] = []
                for i in range(elms):
                    reg[mode].append( tk.StringVar() )
                    reg[mode][i].set("--")
                    width = (mode >> 2) + 2
                    tk.Entry(tab, width=width, state="readonly", textvariable=reg[mode][i]).grid(row=d+1, column=i+1)

            if acc_size is None: continue

            tk.Label(tab, text="acc").grid(row=4)
            self.acc[mode] = []
            for i in range(elms):
                self.acc[mode].append( tk.StringVar() )
                width = (acc_size // 4) + 2
                tk.Entry(tab, width=width, state="readonly", textvariable=self.acc[mode][i]).grid(row=4,column=i+1)


