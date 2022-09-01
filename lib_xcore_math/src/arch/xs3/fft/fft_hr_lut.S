// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


.section    .cp.rodata, "ac", @progbits
.align  4
// In the table below the LSByte indicates the shift behavior
//  0x00 - no shift.  0x40 - left shift. 0x80 - right shift.

#define NEG1_SHL    0xffff0040
#define ZERO_SH0    0x00000000
#define POS1_SHR    0x00010080

.global fft_hr_lut
.cc_top fft_hr_lut
fft_hr_lut:
    .word NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL //  0 -  7
    .word NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL //  8 - 15
    .word NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL // 16 - 23
    .word NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, ZERO_SH0, POS1_SHR, POS1_SHR, POS1_SHR // 24 - 31
.cc_bottom fft_hr_lut



#endif //defined(__XS3A__)