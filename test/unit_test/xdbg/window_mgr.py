

from .gui.register import RegisterWindow
from .gui.vector import VpuWindow
from .gui.command import CommandWindow




class WindowMgr(object):

    TMAP = { 
        "reg": RegisterWindow,
        "vpu": VpuWindow,
    }

    def __init__(self, root, ctrl):
        self.ctrl = ctrl
        self.root = root

        self._active = {}

        self.ctrl.meta_commands["show"] = self.cmd_show
        self.ctrl.meta_commands["hide"] = self.cmd_hide
        self.ctrl.meta_commands["cmd"]  = self.cmd_cmd

    def cmd_show(self, *what):
        for x in what:
            if x not in WindowMgr.TMAP:
                print(f"[WindowMgr| Unknown view: {x}]")
            tp = WindowMgr.TMAP[x]
            if tp in self._active: continue
        
            y = tp(root=self.root, xgdb_ctrl=self.ctrl)
            self._active[tp] = y

    def cmd_hide(self, *what):
        for x in what:
            if x not in WindowMgr.TMAP:
                print(f"[WindowMgr| Unknown view: {x}]")
            tp = WindowMgr.TMAP[x]
            if tp not in self._active: continue
        
            y = self._active[tp]

            y.deinit()
            y.destroy()
            del self._active[tp]

    def cmd_cmd(self, *args):

        cmd = " ".join(args)
        win = CommandWindow(self.root, self.ctrl, cmd)


                
