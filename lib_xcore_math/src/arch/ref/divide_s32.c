// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

static inline int32_t s32_sat_s64(int64_t x) {
    int32_t res = 0;
    if (x > INT32_MAX) {res = INT32_MAX;}
    else if (x < INT32_MIN) {res = INT32_MIN;}
    else { res = (int32_t)x;}
    return res;
}

uint32_t u32_divide_u64_u32(
    const uint64_t x,
    const uint32_t y) {
    uint64_t res = x / y;
    res = (res > UINT32_MAX) ? UINT32_MAX : res;
    return res;
}

int32_t s32_divide_s64_s32(
    const int64_t x,
    const int32_t y) {
    return s32_sat_s64(x/y);
}

int32_t s32_divide_u64_s32(
    const uint64_t x,
    const int32_t y) {
    int64_t res = 0;
    if (y > 0) {
        res = x / y;
    } else {
        res = -(x / -y);
    }
    return s32_sat_s64(res);
}

int32_t s32_divide_s64_u32(
    const int64_t x,
    const uint32_t y) {
    return s32_sat_s64(x/y);
}
