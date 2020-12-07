
import subprocess
import os
import signal
import atexit
import threading
import sys
import time
import re
import tkinter as tk
import traceback


RE_REG_DEFINES = re.compile(r"^#define\s+(\w+)\s+(r\d+)(?:\s+//\s*!\[(.+)\])?") 

assert RE_REG_DEFINES.match(r"#define sample      r1      // ![%d]") is not None
assert RE_REG_DEFINES.match(r"#define coef        r2      // ![0x%08X]") is not None
assert RE_REG_DEFINES.match(r"#define tmp         r3") is not None


def _map_source_file(filepath):
    reg_map = {}

    with open(filepath, "r") as file:
        for ln,line in enumerate(file.readlines()):

            match = RE_REG_DEFINES.match(line)
            if match is None: continue

            name, reg, fmt = match.groups()
            if reg not in reg_map: reg_map[reg] = []
            
            fmt = fmt if fmt is not None else "0x%08X"

            reg_map[reg].append((ln+1,name,fmt))

    # print("@@@@@", reg_map)

    return reg_map


class FileRegisterMap(object):

    _DEFAULT_MAP = { f"r{i}": (f"r{i}","0x%08X") for i in range(12) }
    
    def __init__(self):
        self._map = {}

    def map_file(self, filepath, remap=False):
        if filepath is None:
            return
        if (not remap) and (filepath in self._map):
            return

        try:
            self._map[filepath] = _map_source_file(filepath)
        except Exception:
            traceback.print_exc()

    def get_reg_info(self, filepath, lineno):

        self.map_file(filepath)
        
        if filepath not in self._map:
            return FileRegisterMap._DEFAULT_MAP

        regs = { f"r{i}": (f"r{i}","0x%08X") for i in range(12) }

        for rid,defs in self._map[filepath].items():
            for ln,name,fmt in defs:
                if ln > lineno: break
                regs[rid] = (name,fmt)
        
        return regs


