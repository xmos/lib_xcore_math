
import re
import ctypes
import numpy as np


# RE_INFO_REG_VALUE = re.compile(r"^(r\d+)\s+(0x[0-9a-fA-F]{1,8})\s+(-?\d+)\s+?(.+)?$")
RE_INFO_REG_VALUE = re.compile(r"^(r\d+)\s+(0x[0-9a-fA-F]{1,8})")

assert RE_INFO_REG_VALUE.match("r10            0x5      5") is not None
assert RE_INFO_REG_VALUE.match("r11            0x0      0") is not None
assert RE_INFO_REG_VALUE.match("r4             0x0      0") is not None
assert RE_INFO_REG_VALUE.match("r9             0xfff026c8       -1038648") is not None
assert RE_INFO_REG_VALUE.match("r6             0xfff7c  1048444") is not None
assert RE_INFO_REG_VALUE.match("r2             0xfff00406       -1047546") is not None
assert RE_INFO_REG_VALUE.match("r0             0x80000  524288   _start + 0") is not None


RE_FILE_AND_LINE = re.compile(r"#(\d+)\s+(\S+)\s+\(\)\s+at\s+(.+):(\d+)")

assert RE_FILE_AND_LINE.match("""#0  xs3_filter_biquad_s32 ()
    at C:/repo/lib_xs3_math/lib_xs3_math/src/arch/xcore/filter/xs3_filter_biquad_s32.S:51
51              std r4, r5, sp[1]") is not None""")

# Format is that given by "print/x $vec_r.v8_int32"
RE_PRINTX_VECTOR = re.compile(r"^\$\d+ = \{{{0},\s+{0},\s+{0},\s+{0},\s+{0},\s+{0},\s+{0},\s+{0}\}}".format(r"(0x[0-9a-fA-F]{1,8})"))
assert RE_PRINTX_VECTOR.match("$2 = {0x0, 0x1, 0x2, 0xabc, 0xA9f, 0x0, 0x0, 0x0}") is not None
assert RE_PRINTX_VECTOR.match("""$87 = {0x40000000, 0x4900464e, 0x6e69666e, 0xe7797469, 0x4e414e28, 
0x4e614e00, 0x67756200, 0x206e6920}""") is not None

#given by: "info reg vec_vsr"
RE_VEC_VSR = re.compile(r"vec_vsr\s+(0x[0-9a-fA-F]{1,8})\s+-?\d+")

assert RE_VEC_VSR.match("vec_vsr        0x200      512") is not None

# ctypes documentation says that there are LittleEndianUnion and BigEndianUnion types, but these
# aren't actually implemented. There's an issue about that from 2018.
# This works on my system.
class CVect(ctypes.Union):
    _fields_ = [
        ("int32", ctypes.c_int32 * 8),
        ("int16", ctypes.c_int16 * 16),
        ("int8", ctypes.c_int8 * 32),
        ("uint32", ctypes.c_uint32 * 8),
        ("uint16", ctypes.c_uint16 * 16),
        ("uint8", ctypes.c_uint8 * 32),
    ]


class VpuState(object):
    def __init__(self, vsr, vr, vd, vc):
        self.vsr = vsr
        self.vR = vr
        self.vD = vd
        self.vC = vc 
    
    @property
    def mode(self):
        # 0x0000 -> 32
        # 0x0100 -> 16
        # 0x0200 -> 8
        s = self.vsr >> 8
        return 32 >> s

    @property
    def acc40(self):
        vd = np.array(self.vD.int32).astype(np.uint64) << 32
        vr = np.array(self.vR.int32).astype(np.uint32)
        return (vd+vr).astype(np.uint64)


    @property
    def acc32(self):
        vd = np.array(self.vD.int16).astype(np.uint32) << 16
        vr = np.array(self.vR.int16).astype(np.uint16)
        return (vd+vr).astype(np.uint32)




class util:

    class parse:

        def file_and_line(msg):
            res = RE_FILE_AND_LINE.match(msg)
            if res is None: 
                return None, None
            return res.groups()[2], int(res.groups()[3])

        def register_values(rids, resp):
            regs = {}

            for line in resp.splitlines():

                match = RE_INFO_REG_VALUE.match(line)
                if not match: continue

                qwe = match.groups()
                rid = qwe[0]
                if (len(rids) != 0) and (rid not in rids): continue

                regs[rid] = int(qwe[1],16)

            return regs
        
        def vector_info(msg):
            match = RE_PRINTX_VECTOR.match(msg)
            if match is None:
                raise ValueError(f"Vector info could not be parsed. ({msg})")
            val = [np.uint32(int(x,16)) for x in match.groups()]
            res = CVect()
            for i in range(8):
                res.int32[i] = val[i]
            return res

        def vector_status_reg(msg):
            match = RE_VEC_VSR.match(msg)
            if match is None:
                raise ValueError(f"Vector status register could not be parsed. ({msg})")
            return np.uint32(int(match.group(1),16))


    class get:

        def current_file_and_line(xgdb_ctrl):
            resp = xgdb_ctrl.send_command("frame")
            return util.parse.file_and_line(resp)

            
        def register_values(xgdb_ctrl, rids = []):
            cmd = "info registers " + " ".join(rids)
            resp = xgdb_ctrl.send_command(cmd)
            return util.parse.register_values(rids, resp)

        def vpu_state(xgdb_ctrl):

            vr = xgdb_ctrl.send_command("print/x $vec_r.v8_int32", suppress_prompt=True)
            vd = xgdb_ctrl.send_command("print/x $vec_d.v8_int32", suppress_prompt=True)
            vc = xgdb_ctrl.send_command("print/x $vec_c.v8_int32", suppress_prompt=True)
            vsr = xgdb_ctrl.send_command("info reg vec_vsr", suppress_prompt=True)

            vr = util.parse.vector_info(vr)
            vd = util.parse.vector_info(vd)
            vc = util.parse.vector_info(vc)
            vsr = util.parse.vector_status_reg(vsr)

            return VpuState(vsr, vr, vd, vc)
