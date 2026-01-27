// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  

unsigned vect_s32_argmax(
    const int32_t b[],
    const unsigned length);
*/


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (3)
#define NSTACKWORDS     (8 + 8*NSTACKVECS+4)

#define FUNCTION_NAME       vect_s32_argmax

#define STACK_VEC_MAX_DEX   (NSTACKWORDS-8-4)
#define STACK_VEC_CUR_MAX   (NSTACKWORDS-16-4)
#define STACK_VEC_CUR_DEX   (NSTACKWORDS-24-4)

#define STACK_N     6

#define b           x10      // ![0x%08X]
#define N           x11      // ![%d]
#define vec_8s      x12      // ![0x%X]
#define tmp         x13      // ![%d]
#define tmz         x18      // ![%d]
#define cur_max     x19      // ![0x%08X]
#define mask_0xF    x20      // ![0x%04X]



FUNCTION_NAME:

    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16

{   li t3, 0                              ;   sw N, (STACK_N)*4                      (sp)}
{   srli N, N, 3                             ;   xm.vsetc t3}

// cur_max[i] = -0x80000000
    la t3, vpu_vec_0x80000000
{   addi t3,sp, (STACK_VEC_CUR_MAX)*4         ;   xm.vldr t3}
{   xm.mkmski mask_0xF, 4                       ;   xm.vstr t3}

// cur_dex[i] = i
{   addi tmp,sp, (STACK_VEC_CUR_DEX)*4         ;   li t3, 7                             }
.L_setup_cur_dex:
xm.stw t3, t3(tmp)  
    {   addi t3, t3, -1                     ;   xm.bt t3, .L_setup_cur_dex                    }

// max_dex[i] = -1
    la t3, vpu_vec_neg_1
{   addi t3,sp, (STACK_VEC_MAX_DEX)*4         ;   xm.vldr t3}
{   addi cur_max,sp, (STACK_VEC_CUR_MAX)*4     ;   xm.vstr t3}

    la t3, vpu_vec_0x00000008
{   mv vec_8s, t3                         ;   xm.vclrdr                                  }
{   mv t3, b                              ;   xm.brff N, .L_loop_bot                       }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

.L_loop_top:
    {   mv b, t3                              ;   xm.vldr t3}
    {   addi N, N, -1                             ;   xm.vlsub cur_max}
    {   addi t3,sp, 0                         ;   xm.vdepth1                                 }
        xm.vstrpv t3, mask_0xF
    {   mv t3, b                              ;   lw tmp, 0                          (sp)}
    {   mv tmz, tmp                            ; nop                                           }
xm.zip tmz, tmp, 0
    {   mv tmz, tmp                            ; nop                                           }
xm.zip tmz, tmp, 0
    {   addi t3,sp, (STACK_VEC_CUR_DEX)*4         ;   xm.vldr t3}
        xm.vstrpv cur_max, tmp
    {   addi tmz,sp, (STACK_VEC_MAX_DEX)*4         ;   xm.vldr t3}
        xm.vstrpv tmz, tmp      
    { nop                                           ;   xm.vladd vec_8s}
    {   li t3, 32                             ;   xm.vstr t3}
    {   add t3, b, t3                         ;   xm.bt N, .L_loop_top                       }
.L_loop_bot:

{ nop                                           ;   lw N, (STACK_N)*4                      (sp)}
{   xm.zexti N, 3                               ; nop                                           }
{   xm.mkmsk N, N                              ;   xm.brff N, .L_no_tail                        }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
{   mv b, t3                              ;   xm.vldr t3}
{   addi t3,sp, 0                         ;   xm.vlsub cur_max}
{ nop                                           ;   xm.vdepth1                                 }
    xm.vstrpv t3, mask_0xF
{   mv t3, b                              ;   lw tmp, 0                          (sp)}
{   and tmp, tmp, N                         ;   and tmz, tmp, N                         }
xm.zip tmz, tmp, 0
{   mv tmz, tmp                            ; nop                                           }
xm.zip tmz, tmp, 0
{   addi t3,sp, (STACK_VEC_CUR_DEX)*4         ;   xm.vldr t3}
    xm.vstrpv cur_max, tmp
{   addi tmz,sp, (STACK_VEC_MAX_DEX)*4         ;   xm.vldr t3}
    xm.vstrpv tmz, tmp      

.L_no_tail:

#undef cur_max
#undef vec_8s
#undef mask_0xF

#define cur_max     x19  // ![%d]
#define max_dex     x12  // ![0x%08X]

{   addi t3,sp, (STACK_VEC_CUR_MAX)*4         ;   li N, 7                                }
{   addi s4,sp, (STACK_VEC_MAX_DEX)*4          ;   xm.ldw cur_max,N            ( t3)}
{   addi N, N, -1                             ;   xm.ldw max_dex,N                      ( s4)}
.L_loop2_top:
    { nop                                           ;   xm.ldw a0,N                         ( t3)}
    {   xm.slt tmp, a0, cur_max                    ;   xm.ldw s5,N                        ( s4)}
  xm.eq tmz, a0, cur_max                     
{ nop ;   xm.bt tmp, .L_less_than                    }
    .L_greater_or_equal:
        {    xm.slt tmp, s5, max_dex                   ;   xm.brff tmz, .L_greater                      }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
        .L_equal:
            { nop                                           ;   xm.brff tmp, .L_less_than                    }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
        .L_greater:
            {   mv cur_max, a0                         ;   mv max_dex, s5                         }

    .L_less_than:
    {   addi N, N, -1                             ;   xm.bt N, .L_loop2_top                      }

    xm.lddsp  s5,s4,16
    xm.lddsp  s3,s2,8
{   mv a0, max_dex                         ;   xm.retsp (NSTACKWORDS)*4                       }


.L_end: 


.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__VX4B__)
