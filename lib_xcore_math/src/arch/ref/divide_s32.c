#include <stdint.h>
#include <stdio.h>

int32_t u32_divide_u64_u32_ref(
    const uint64_t x,
    const uint32_t y) {
    if ((x >> 32) >= y) {
        return 0xffffffff;
    }
    return x/y;
}

int32_t s32_divide_s64_s32_ref(
    const int64_t x,
    const int32_t y) {
    if (x < 0) {
        if (y < 0) {
            if (((-x) >> 32) >= -y) {
                return 0xffffffff;
            }
        } else {
            if (((-x) >> 32) >= y) {
                return 0x00000001;
            }
        }
    } else {
        if (y < 0) {
            if (((x) >> 32) >= -y) {
                return 0x00000001;
            }
        } else {
            if (((x) >> 32) >= y) {
                return 0xffffffff;
            }
        }
    }
    return x/y;
}

int32_t s32_divide_u64_s32_ref(
    const uint64_t x,
    const int32_t y) {
    if (y > 0) {
        if ((x >> 32) >= (uint32_t)y) {
            return 0xffffffff;
        }
        return x/y;
    } else {
        if ((x >> 32) >= (uint32_t)-y) {
            return 0xffffffff;
        }
        return -(x/-y);
    }
    printf("%lld / %d = %d?\n", x, y, (int)x/y);
    return x/y;
}

int32_t s32_divide_s64_u32_ref(
    const int64_t x,
    const uint32_t y) {
    if (x > 0) {
        return u32_divide_u64_u32_ref(x, y);
    } else {
        return -u32_divide_u64_u32_ref(-x, y);
    }
}
