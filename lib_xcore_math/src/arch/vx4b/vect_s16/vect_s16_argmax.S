// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  

unsigned vect_s16_argmax(
    const int16_t b[],
    const unsigned length);
*/


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (3)
#define NSTACKWORDS     (8 + 8*NSTACKVECS+4)

#define FUNCTION_NAME       vect_s16_argmax

#define STACK_VEC_MAX_DEX   (NSTACKWORDS-8-4)
#define STACK_VEC_CUR_MAX   (NSTACKWORDS-16-4)
#define STACK_VEC_CUR_DEX   (NSTACKWORDS-24-4)

#define STACK_N     6

#define b           x10      // ![0x%08X]
#define N           x11      // ![%d]
#define vec_16s     x12      // ![0x%X]
#define tmp         x13      // ![%d]
#define tmz         x18      // ![%d]
#define cur_max     x19      // ![0x%08X]
#define mask_0xF    x20      // ![0x%04X]


FUNCTION_NAME:

    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16

    li t3, 0x100  //16-bit mode
{   xm.mkmski mask_0xF, 4                       ;   sw N, (STACK_N)*4                      (sp)}
{   srli N, N, 4                             ;   xm.vsetc t3}

// cur_max[i] = -0x8000
    la t3, vpu_vec_0x8000
{   addi t3,sp, (STACK_VEC_CUR_MAX)*4         ;   xm.vldr t3}
{ nop                                           ;   xm.vstr t3}

// cur_dex[i] = i
{   addi tmp,sp, (STACK_VEC_CUR_DEX)*4         ;   li t3, 15                             }
.L_setup_cur_dex:
        xm.st16 t3,  t3(tmp)
    {   addi t3, t3, -1                     ;   xm.bt t3, .L_setup_cur_dex                    }

// max_dex[i] = -1
    la t3, vpu_vec_neg_1
{   addi t3,sp, (STACK_VEC_MAX_DEX)*4         ;   xm.vldr t3}
{   addi cur_max,sp, (STACK_VEC_CUR_MAX)*4     ;   xm.vstr t3}

    la t3, vpu_vec_0x0010
{   mv vec_16s, t3                        ;   xm.vclrdr                                  }
{   mv t3, b                              ;   xm.brff N, .L_loop_bot                       }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */


/*
    - compute  cur_max[k] - b[k]
    - create a mask from values less than 0
        cur_max[k] - b[k] < 0  -->  b[k] > cur_max[k]
    - overwrite cur_max[k] and max_dex[k] where  b[k] > cur_max[k]
    - increment cur_dex[k] by 1 (all cur_dex[] elements are  the same)
    - repeat on next 16 elements of b[]

    Note: This replaces max_dex[k] when b[k] > cur_max[k], NOT when b[k] >= cur_max[k],
          so this loop prefers earlier indexes

*/
.L_loop_top:
    {   mv b, t3                              ;   xm.vldr t3}
    {   addi N, N, -1                             ;   xm.vlsub cur_max}
    {   addi t3,sp, 0                         ;   xm.vdepth1                                 }
        xm.vstrpv t3, mask_0xF
    {   mv t3, b                              ;   lw tmp, 0                          (sp)}
    {   mv tmz, tmp                            ; nop                                           }
xm.zip tmz, tmp, 0                                                                      
    {   addi t3,sp, (STACK_VEC_CUR_DEX)*4         ;   xm.vldr t3}
        xm.vstrpv cur_max, tmp                                                                
    {   addi tmz,sp, (STACK_VEC_MAX_DEX)*4         ;   xm.vldr t3} 
        xm.vstrpv tmz, tmp                                                                    
    { nop                                           ;   xm.vladd vec_16s}
    {   li t3, 32                             ;   xm.vstr t3} 
    {   add t3, b, t3                         ;   xm.bt N, .L_loop_top                       }
.L_loop_bot:

{ nop                                           ;   lw N, (STACK_N)*4                      (sp)}
{   xm.zexti N, 4                               ; nop                                           }
{   xm.mkmsk N, N                              ;   xm.brff N, .L_no_tail                        }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
{   mv b, t3                              ;   xm.vldr t3}
{   addi t3,sp, 0                         ;   xm.vlsub cur_max}
{ nop                                           ;   xm.vdepth1                                 }
    xm.vstrpv t3, mask_0xF
{   mv t3, b                              ;   lw tmp, 0                          (sp)}
{   and tmp, tmp, N                         ;   and tmz, tmp, N                         }
xm.zip tmz, tmp, 0
{   addi t3,sp, (STACK_VEC_CUR_DEX)*4         ;   xm.vldr t3}
    xm.vstrpv cur_max, tmp
{   addi tmz,sp, (STACK_VEC_MAX_DEX)*4         ;   xm.vldr t3}
    xm.vstrpv tmz, tmp      
    
.L_no_tail:

#undef cur_max
#undef vec_16s
#undef mask_0xF

#define cur_max     x19  // ![%d]
#define max_dex     x12  // ![0x%08X]

{   addi t3,sp, (STACK_VEC_CUR_MAX)*4         ;   li N, 15                               }
{xm.ldawsp x20, STACK_VEC_MAX_DEX *4 ; nop} 
xm.ld16s cur_max, N(x28)
xm.ld16s max_dex,  N(x20)
{   addi N, N, -1                             ;   nop}
.L_loop2_top:                                         ;   
xm.ld16s x10, N(x28)
slt tmp, x10, cur_max  
xm.ld16s x21, N(x20)
xm.eq tmz, a0, cur_max                     
{nop ; xm.bt tmp, .L_less_than                    }
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
