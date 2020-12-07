

import re
import numpy as np

from .io import XgdbIO, XgdbUserInput
from .reg_map import FileRegisterMap



def _register_meta_commands(ctrl):
    pass


class XgdbController(object):
    
    def __init__(self, xgdb_proc):

        self.xgdb_proc = xgdb_proc
        self.io = XgdbIO(xgdb_proc)
        self.meta_commands = {}
        self.file_reg_map = FileRegisterMap()

        self._refreshers = set()

        _register_meta_commands(self)

        self.user_input = XgdbUserInput(self._tx_xgdb_command, self._do_meta_command)

        self.user_input.start()
        self.io.start_threads()

    def add_refresh_cb(self, refresh_cb):
        self._refreshers.add(refresh_cb)
    
    def remove_refresh_cb(self, refresh_cb):
        self._refreshers.remove(refresh_cb)

    def _tx_xgdb_command(self, cmd_str):
        self.send_command(cmd_str, True, False)

        for cb in self._refreshers:
            cb(self)
        
        if len(self._refreshers):
            self.send_command(" ")

    def _do_meta_command(self, cmd_name, args):
        if cmd_name not in self.meta_commands:
            print(f"Unknown meta command: @{cmd_name}")
        else:
            self.meta_commands[cmd_name](*args)
        
        self.send_command(" ", True, False)
    
    def send_command(self, command=None, print_response=False, clear_replay=False, suppress_prompt=True):
        return self.io.send_command(command, print_response, clear_replay, suppress_prompt)

    def wait_for_prompt(self):
        self.send_command(print_response=True)
