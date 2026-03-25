#include <stdint.h>
#include <stdio.h>

int32_t u32_divide_u64_u32(
    const uint64_t x,
    const uint32_t y) {
    return x/y;
}

int32_t s32_divide_s64_s32(
    const int64_t x,
    const int32_t y) {
    return x/y;
}

int32_t s32_divide_u64_s32(
    const uint64_t x,
    const int32_t y) {
    if (y > 0) {
        return x/y;
    } else {
        return -(x/-y);
    }
    return x/y;
}

int32_t s32_divide_s64_u32(
    const int64_t x,
    const uint32_t y) {
    if (x > 0) {
        return u32_divide_u64_u32(x, y);
    } else {
        return -u32_divide_u64_u32(-x, y);
    }
}
