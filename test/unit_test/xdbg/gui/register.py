
from xdbg.util import util

import numpy as np
import tkinter as tk


class RegisterTable: 
    def __init__(self, root):

        self.reg_map = {}

        for i in range(12):

            rid = f"r{i}"
            
            reg_name = tk.Label(root, width=9, text=rid)
            reg_val = tk.Label(root, width=20, text="")

            self.reg_map[rid] = (reg_name, reg_val)

            reg_name.grid(row=i, column=0, stick=tk.W)
            reg_val.grid(row=i, column=1)

    def set_reg(self, rid, value, name=None):
        if rid not in self.reg_map:
            return

        name = "{0} ({1})".format(name,rid) if name and (name != rid) else rid
        self.reg_map[rid][0].config(text = name)
        self.reg_map[rid][1].config(text = value)

    def update_registers(self, info_map, value_map):
        for rid in self.reg_map:
            name,fmt = info_map[rid] if rid in info_map else (rid,"0x%08X")
            val = fmt % (value_map[rid]) if rid in value_map else "[unknown]"

            self.set_reg(rid, val, name)
    

class RegisterWindow(tk.Toplevel):

    def __init__(self, root, xgdb_ctrl):
        tk.Toplevel.__init__(self, master=root)

        self.ctrl = xgdb_ctrl

        self.title("Registers")
        self.attributes('-topmost', True)
        self.resizable(False, False)
        # self.overrideredirect(1)

        self.reg_table = RegisterTable(self)

        self.ctrl.add_refresh_cb(self.on_refresh)

        self.on_refresh(self.ctrl)

    def deinit(self):
        self.ctrl.remove_refresh_cb(self.on_refresh)

    def on_refresh(self, ctrl, *args):
        
        filepath, lineno = util.get.current_file_and_line(ctrl)
        
        reg_info = ctrl.file_reg_map.get_reg_info(filepath, lineno)
        reg_values = util.get.register_values(ctrl)

        self.reg_table.update_registers(reg_info, reg_values)

