

import threading
import os
import sys
import traceback

XGDB_PROMPT = "(gdb) "

def start_daemon(func, *args):
    t = threading.Thread(target=func, args=args)
    t.daemon = True
    t.start()
    return t


def redirect_stream(from_stream, to_stream, read_line=False):
    while True:
        if read_line: s = from_stream.readline()
        else:         s = from_stream.read(1)
        
        to_stream.write(s)
        to_stream.flush()


class XgdbIO(object):
    
    def __init__(self, xgdb_process):
        self.xgdb_proc = xgdb_process
        self._read_pipe = None
        self._cmd_lock = threading.RLock()


    def send_command(self, command=None, print_response=False, clear_replay=False, suppress_prompt=False):
        
        with self._cmd_lock:
            if command is not None:
                self.xgdb_proc.stdin.write(command)
                self.xgdb_proc.stdin.write("\n")

            resp = ""
            while resp[-len(XGDB_PROMPT):] != XGDB_PROMPT:
                ch  = self._read_pipe.read(1)
                if print_response:
                    sys.stdout.write(ch)
                resp = resp + ch

            if print_response:
                sys.stdout.flush()

            if clear_replay:
                self.send_command(" ", print_response=print_response, clear_replay=False)

        if suppress_prompt:
            resp = resp[:-len(XGDB_PROMPT)-1]

        return resp

    def start_threads(self):
        read_fd, write_fd = os.pipe()

        self._read_pipe = open(read_fd, "r")
        writer = open(write_fd, "w")

        self._stdout_thread = start_daemon(redirect_stream, self.xgdb_proc.stdout, writer)
        self._stderr_thread = start_daemon(redirect_stream, self.xgdb_proc.stderr, sys.stdout, True)


            
class XgdbUserInput(threading.Thread):
    def __init__(self, xgdb_cmd_cb, meta_cmd_cb):
            threading.Thread.__init__(self)
            self.daemon = True

            self.xgdb_cmd_cb = xgdb_cmd_cb
            self.meta_cmd_cb = meta_cmd_cb


    def _handle_command(self, cmd):

        if (len(cmd) > 0) and (cmd[0] == "@"):
            # Is a meta command
            tokens = cmd[1:].split()
            if len(tokens) == 0:
                return
            
            self.meta_cmd_cb(tokens[0], tokens[1:])
        else:
            # Is a command for xgdb
            self.xgdb_cmd_cb(cmd.replace("\n",""))

    def run(self):
        last_cmd = " "
        while True:
            try:
                cmd = sys.stdin.readline()
                if cmd == "\n":
                    cmd = last_cmd
                self._handle_command(cmd)
                last_cmd = cmd
            except Exception as e:
                traceback.print_exc()
            