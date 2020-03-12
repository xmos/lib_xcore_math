
#ifndef ASM_HELPER_H_
#define ASM_HELPER_H_

#include "../../api/xs3_math_conf.h"

#define EPV_LOG2_S8     5
#define EPV_LOG2_S16    4
#define EPV_LOG2_S32    3
#define EPV_LOG2_C32    2

#define SIZEOF_LOG2_S8  0
#define SIZEOF_LOG2_S16 1
#define SIZEOF_LOG2_S32 2
#define SIZEOF_LOG2_C32 3

#define HR_SUB_S8   7
#define HR_SUB_S16  15
#define HR_SUB_S32  31

#define ASM_PREAMBLE(FNAME)                         \
    .text;                                          \
    .issue_mode dual;                               \
    .globl FNAME;                                   \
    .type FNAME,@function;                          \
    .align 16;                                      \
    .cc_top FNAME.function,FNAME;                   \


#define CAT_(A, B)    A##B
#define CAT(A, B)     CAT_(A,B)

#define ASM_POSTAMBLE(FNAME, NSTACKWORDS)                                 \
    .cc_bottom FNAME.function;                                            \
    .set FNAME.nstackwords,NSTACKWORDS;     .global FNAME.nstackwords;    \
    .set FNAME.maxcores,1;                  .global FNAME.maxcores;       \
    .set FNAME.maxtimers,0;                 .global FNAME.maxtimers;      \
    .set FNAME.maxchanends,0;               .global FNAME.maxchanends;    \
    CAT(.L_size_end_, FNAME):                                             \
    .size FNAME, CAT(.L_size_end_, FNAME) - FNAME


#define ASM_PREAMBLE_V2(FNAME)          \
    .global FNAME;                      \
    .type FNAME,@function;              \
    .align 16;                          \
    .cc_top FNAME.function,FNAME


#define ASM_PREAMBLE_LCL(FNAME)                         \
    .type CAT(.L_,FNAME),@function;                     \
    .align 16;                                          \
    .cc_top CAT(.L_,FNAME).function,CAT(.L_,FNAME)

#define ASM_POSTAMBLE_LCL(FNAME)                                \
    CAT(.L_end_,FNAME):                                         \
    .cc_bottom CAT(.L_,FNAME).function;                         \
    .size CAT(.L_,FNAME), CAT(.L_end_,FNAME) - CAT(.L_,FNAME)
    
#endif // ASM_HELPER_H_