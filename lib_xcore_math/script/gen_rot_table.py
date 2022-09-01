# Copyright 2020-2022 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
import numpy as np

ITERS_32 = 30

ITERS_16 = 14

SAT_16 = 15

angles = np.zeros(shape=ITERS_32 + 1)
angles[0] = -np.pi / 2

for k in range(1, ITERS_32 + 1):
    angles[k] = angles[k - 1] / 2

angles = angles[1:]
zees = np.exp(1j * angles)

table32 = np.zeros(shape=(ITERS_32, 2), dtype=np.int32)
table16 = np.zeros(shape=(ITERS_16, 2), dtype=np.int16)

table32[:, 0] = np.round(zees.real * (2 ** 30))
table32[:, 1] = np.round(zees.imag * (2 ** 30))

table16[:, 0] = np.round(zees[:ITERS_16].real * (2 ** SAT_16))
table16[:, 1] = np.round(zees[:ITERS_16].imag * (2 ** SAT_16))

# tmp = (table16 == -32768)
# table16[tmp] = 32767

print("////// 32-bit")

print("{")
for k in range(ITERS_32):
    stred = f"{{{table32[k,0]}, {table32[k,1]}}}"

    print(f"  {{ {stred}, {stred}, {stred}, {stred} }},")


print("};")


print("\n\n\n////// 16-bit")

print("{")
for k in range(ITERS_16):
    real = ", ".join((16 * [" 0x%04X" % (table16[k, 0])]))
    imag = ", ".join((16 * ["-0x%04X" % (-table16[k, 1])]))

    print(f"  {{ {{ {real} }},\n    {{ {imag} }} }},")


print("};")


real = 0x4000
imag = 0x0000

for row in range(ITERS_16):

    tab_real = table16[row][0]
    tab_imag = table16[row][1]

    acc = 0
    vC = -imag
    acc += vC * tab_imag
    vC = real
    acc += vC * tab_real

    real = (acc + (1 << 14)) >> 15

    acc = 0
    acc += vC * tab_imag
    vC = imag
    acc += vC * tab_real

    imag = (acc + (1 << 14)) >> 15

    imag = imag if (imag >= 0) else -imag

    print(f"{real}\t\t{imag}")


print(f"Result: {real}\t\tExpected: {0x4000}\t\tDelta: {real - 0x4000}")
