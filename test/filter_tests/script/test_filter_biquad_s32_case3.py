# Copyright 2020-2022 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
import numpy as np

Q30 = lambda x: np.int64(np.round(x * (2 ** 30)))
F30 = lambda x: x / 2 ** 30


def mul_q30(x, y):
    p = ((x * y) + (1 << 29)) >> 30
    assert (-0x80000000 <= p) and (p < 0x80000000)
    return p


def macc_q30(acc, x, y):
    p = mul_q30(x, y)
    acc += p
    assert (-0x80000000 <= acc) and (acc < 0x80000000)
    return acc


def biquad_q30(smp_in, state, coef):
    acc = 0
    for k in [4, 3, 2, 1]:
        acc = macc_q30(acc, coef[k], state[k - 1])
    acc = macc_q30(acc, coef[0], smp_in)

    state[1] = state[0]
    state[0] = smp_in
    state[3] = state[2]
    state[2] = acc
    return acc


def biquad(smp_in, state, coef):
    tmp = coef[0] * smp_in
    tmp += coef[1] * state[0] + coef[2] * state[1]
    tmp += coef[3] * state[2] + coef[4] * state[3]
    state[1] = state[0]
    state[0] = smp_in
    state[3] = state[2]
    state[2] = tmp
    return tmp


def biquads(smp_in, N, states, coefs):
    for k in range(N):
        smp_in = biquad(smp_in, states[k], coefs[k])
    return smp_in


def biquads_q30(smp_in, N, states, coefs):
    for k in range(N):
        smp_in = biquad_q30(smp_in, states[k], coefs[k])
    return smp_in


def random_biquad():
    coef = np.zeros(5)
    coef[0:3] = np.random.random(3) / 5
    coef[3:5] = -np.random.random(2) / 5
    # coef[3:5] = 0.0
    # Make sure it's Q30-compatible
    coef = np.round(coef * (2 ** 30)) / (2 ** 30)
    return coef


N_biquads = 8

coef = np.ones(shape=(N_biquads, 5))

for k in range(N_biquads):
    coef[k, :] = random_biquad()

coef_q30 = Q30(coef)

state = np.zeros(shape=(N_biquads, 4))
state_q30 = np.zeros(shape=(N_biquads, 4), dtype=np.int64)


N_samples = np.random.randint(0, 200)
# N_samples = 10

samples = np.random.randint(low=-(2 ** 30), high=2 ** 30, size=N_samples)

y = np.zeros(shape=samples.shape)
y_q30 = np.zeros(shape=samples.shape, dtype=np.int64)


# print(f"N_samples: {N_samples}")

for i in range(len(samples)):
    y[i] = biquads(samples[i], N_biquads, state, coef)
    y_q30[i] = biquads_q30(samples[i], N_biquads, state_q30, coef_q30)

diff = y - y_q30


# print("y[] = ", y)
# print("y_q30[] = ", y_q30)
# print("\n\n")

asd = np.unravel_index(np.argmax(np.abs(diff)), diff.shape)

print(f"Max absolute difference: y[{asd}] - y_q30[{asd}] = {y[asd]-y_q30[asd]}")

# print(f"Badness:  {np.max(np.abs(diff)) / np.mean(np.abs(y))}"     )

# print(diff)

print("\n\n")


pcoef = np.zeros(shape=(5, 8), dtype=np.int64)
pcoef[:, :N_biquads] = coef_q30.transpose()


def array_to_string(arr, val_fmtr=str):
    if len(arr.shape) == 1:
        return f"{{{', '.join([val_fmtr(x) for x in arr])}}}"
    else:
        return (
            "{"
            + ",".join([array_to_string(arr[i], val_fmtr) for i in range(arr.shape[0])])
            + "}"
        )


def print_arr_c(name, arr, val_fmtr=str):
    dims = "".join([f"[{val_fmtr(x)}]" for x in arr.shape])
    print(f"int32_t {name}{dims} = {array_to_string(arr,val_fmtr=val_fmtr)};")


print("    ", end="")
print_arr_c("samples", samples.astype(np.int64))
print("    ", end="")
print_arr_c("coef", pcoef, val_fmtr=hex)
print("    ", end="")
print_arr_c("Y_exp", y_q30.astype(np.int64))


# print( "    filter_biquad_s32_t filter;")
# print(f"    filter.biquad_count = {N_biquads};")
# print( "    memset(&filter.state, 0, sizeof(filter.state));")
# print( "    memcpy(&filter.coef, coef, sizeof(coef));")

