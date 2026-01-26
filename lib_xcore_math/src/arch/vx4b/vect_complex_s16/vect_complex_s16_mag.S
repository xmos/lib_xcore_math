// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)

#include "../asm_helper.h"

/*  

headroom_t vect_complex_s16_mag(
    int16_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t b_shr,
    const int16_t* rot_table,
    const unsigned table_rows)

*/

.text
.p2align 2

#define NSTACKVECS      (8)
#define NSTACKWORDS     (8+(8*NSTACKVECS)+4)


#define STACK_VEC_TMP_IMAG  (NSTACKWORDS-8-4)
#define STACK_VEC_TMP_REAL  (NSTACKWORDS-16-4)
#define STACK_VEC_TMP2      (NSTACKWORDS-24-4)
#define STACK_VEC_NEG_ONES  (NSTACKWORDS-40-4)
#define STACK_VEC_SAT       (NSTACKWORDS-32-4)

#define FUNCTION_NAME vect_complex_s16_mag

#define Q(R)    R

#define a           x10 
#define b_real      x11 
#define b_imag      x12
#define length      x13
#define b_shr       x18
#define _32         x19
#define vec_neg_one x20
#define mask32      x21
#define tail_bytes  x22
#define iter        x23
#define table       x24

FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16
    xm.stdsp  s7,s6,24

    {   li _32, 32                             ;   sw s8, 4                          (sp)}

    {   slli t3, _32, 3                         ;   mv tail_bytes, length                  }
    {   srli length, length, 4                   ;   xm.vsetc t3}
    { nop                                           ;   li t3, 15                             }

    {   slli s8, t3, 16                        ;   xm.zexti tail_bytes, 4                      }
    {   or t3, t3, s8                        ;   xm.mkmski mask32, 32                        }
    xm.stdsp  t3,t3,(STACK_VEC_SAT/2 + 0)*8
    xm.stdsp  t3,t3,(STACK_VEC_SAT/2 + 1)*8
    xm.stdsp  t3,t3,(STACK_VEC_SAT/2 + 2)*8
    xm.stdsp  t3,t3,(STACK_VEC_SAT/2 + 3)*8
    li s8, 0xC000
    {   slli s7, s8, 16                         ;   slli tail_bytes, tail_bytes, 1           }
    {   or s8, s8, s7                         ;   nop}
    addi vec_neg_one, sp, STACK_VEC_NEG_ONES*4

    xm.stdi  s8,s8, 0(vec_neg_one)
    xm.stdi  s8,s8, 8(vec_neg_one)
    xm.stdi  s8,s8, 16(vec_neg_one)
    xm.stdi  s8,s8, 24(vec_neg_one)

    {   xm.mkmsk tail_bytes, tail_bytes            ;   nop}
    mv b_shr, a4
    {   addi t3,sp, (STACK_VEC_TMP_REAL)*4        ;   nop            }/* Multiple XAT warnings: 'LDAWSP outside of known frame - offset may need correction', 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    beqz length, .L_outer_loop_bot 

    .L_outer_loop_top:
            xm.vlashr b_real, b_shr
            xm.vstrpv t3, mask32
        {   add b_real, b_real, _32                 ;   xm.vsign                                   }
        addi Q(iter),sp, (STACK_VEC_TMP_IMAG)*4    
        {nop ;   xm.vlmul0 t3}
        xm.vlmul1 t3
        xm.vstrpv t3, mask32

            {nop; xm.vladd t3} ///
            xm.vstrpv t3, mask32 ///


            xm.vlashr b_imag, b_shr
            xm.vstrpv Q(iter), mask32
        {   add b_imag, b_imag, _32                 ;   xm.vsign                                   }
        { nop                                           ;   xm.vlmul0 Q(iter)}
        xm.vlmul1 Q(iter)
        xm.vstrpv Q(iter), mask32

            {nop; xm.vladd Q(iter)} ///
            xm.vstrpv Q(iter), mask32 ///


        mv table, a5
         addi t3,sp, (STACK_VEC_TMP_IMAG)*4        
         mv iter, a6

        .L_inner_loop_top:
           // {addi t3, sp, STACK_VEC_NEG_ONES*4; xm.vclrdr}


            { nop                                           ;   xm.vldr t3}
            {   addi t3,sp, (STACK_VEC_TMP2)*4            ;   xm.vlmul0 vec_neg_one}
            xm.vlmul1 vec_neg_one
                xm.vstrpv t3, mask32

                {nop; xm.vladd t3} ///
                xm.vstrpv t3, mask32 ///


            { nop                                           ;   xm.vclrdr                                  }
            {   add table, table, _32                   ;   xm.vldc t3}
            {   addi t3,sp, (STACK_VEC_TMP_REAL)*4        ;   xm.vlmacc0 table}
            xm.vlmacc1 table
            {   sub table, table, _32                   ;   xm.vldc t3}
            {   addi t3,sp, (STACK_VEC_SAT)*4             ;   xm.vlmacc0 table}
            xm.vlmacc1 table
            xm.vlsat t3
             addi t3,sp, (STACK_VEC_TMP_REAL)*4  
            
                xm.vstrpv t3, mask32
            {   add table, table, _32                   ;   xm.vclrdr                                  }
            addi t3,sp, (STACK_VEC_TMP_IMAG)*4       
            {nop ;   xm.vlmacc0 table}
            xm.vlmacc1 table
            {   sub table, table, _32                   ;   xm.vldc t3}
            {   addi t3,sp, (STACK_VEC_SAT)*4             ;   xm.vlmacc0 table}
            xm.vlmacc1 table
            xm.vlsat t3
             addi t3,sp, (STACK_VEC_TMP_IMAG)*4     
            
                xm.vstrpv t3, mask32   
            {   add table, table, _32                   ;   xm.vsign                                   }
            {   addi iter, iter, -1                       ;   xm.vlmul0 t3} // imag = |imag|
            xm.vlmul1 t3
                xm.vstrpv t3, mask32

                {nop; xm.vladd t3} ///
                xm.vstrpv t3, mask32 ///

            {   add table, table, _32                   ;   xm.bt iter, .L_inner_loop_top              }

        {   addi t3,sp, (STACK_VEC_TMP_REAL)*4        ; nop                                           }
        {   addi length, length, -1                   ;   xm.vldr t3}
        {   add a, a, _32                           ;   xm.vstr a}
        bnez length, .L_outer_loop_top
    .L_outer_loop_bot:  

    {   addi t3,sp, (STACK_VEC_TMP_REAL)*4        ;  nop                }/* Multiple XAT warnings: 'LDAWSP outside of known frame - offset may need correction', 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    beqz tail_bytes, .L_done
        xm.vlashr b_real, b_shr
        xm.vstrpv t3, tail_bytes
    {   add b_real, b_real, _32                 ;   xm.vsign                                   }
    addi Q(iter),sp, (STACK_VEC_TMP_IMAG)*4    
    {nop ;   xm.vlmul0 t3}
    xm.vlmul1 t3
    xm.vstrpv t3, mask32

                {nop; xm.vladd t3} ///
                xm.vstrpv t3, mask32 ///

        xm.vlashr b_imag, b_shr
        xm.vstrpv Q(iter), tail_bytes
    {   add b_imag, b_imag, _32                 ;   xm.vsign                                   }
    { nop                                           ;   xm.vlmul0 Q(iter)}
    xm.vlmul1 Q(iter)
    xm.vstrpv Q(iter), mask32

                {nop; xm.vladd Q(iter)} ///
                xm.vstrpv Q(iter), mask32 ///


    mv table, a5
    addi t3,sp, (STACK_VEC_TMP_IMAG)*4     
    mv iter, a6 

    .L_inner_loop2_top:
        // {   ldaw x28, sp[STACK_VEC_NEG_ONES]        ;   vclrdr                                  }
        addi t3,sp, (STACK_VEC_TMP_IMAG)*4        
        {nop;   xm.vldr t3}
        {   addi t3,sp, (STACK_VEC_TMP2)*4            ;   xm.vlmul0 vec_neg_one}
         xm.vlmul1 vec_neg_one
         xm.vstrpv t3, mask32

                {nop; xm.vladd t3} ///
                xm.vstrpv t3, mask32 ///

        { nop                                           ;   xm.vclrdr                                  }
        {   add table, table, _32                   ;   xm.vldc t3}
        {   addi t3,sp, (STACK_VEC_TMP_REAL)*4        ;   xm.vlmacc0 table}
        xm.vlmacc1 table
        {   sub table, table, _32                   ;   xm.vldc t3}
        {   addi t3,sp, (STACK_VEC_SAT)*4             ;   xm.vlmacc0 table}
        xm.vlmacc1 table
         xm.vlsat t3
         addi t3,sp, (STACK_VEC_TMP_REAL)*4    
        
            xm.vstrpv t3, mask32
        {   add table, table, _32                   ;   xm.vclrdr                                  }
        addi t3,sp, (STACK_VEC_TMP_IMAG)*4     
        {nop   ;   xm.vlmacc0 table}
        xm.vlmacc1 table
        {   sub table, table, _32                   ;   xm.vldc t3}
        {   addi t3,sp, (STACK_VEC_SAT)*4             ;   xm.vlmacc0 table}
        xm.vlmacc1 table
        xm.vlsat t3
         addi t3,sp, (STACK_VEC_TMP_IMAG)*4  
        
            xm.vstrpv t3, mask32   
        {   add table, table, _32                   ;   xm.vsign                                   }
        {   addi iter, iter, -1                       ;   xm.vlmul0 t3} // imag = |imag|
        xm.vlmul1 t3
        xm.vstrpv t3, mask32

                {nop; xm.vladd t3} ///
                xm.vstrpv t3, mask32 ///

        {   add table, table, _32                   ;   xm.bt iter, .L_inner_loop2_top             }

    {   addi t3,sp, (STACK_VEC_TMP_REAL)*4        ;   xm.vclrdr                                  }
    { nop                                           ;   xm.vldr t3}
    { nop                                           ;   xm.vstd t3}
        xm.vstrpv t3, tail_bytes
        xm.vstrpv a, tail_bytes
    { nop                                           ;   xm.vldd t3}
    { nop                                           ;   xm.vstd t3}


.L_done:
        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
        xm.lddsp  s7,s6,24

    {   li a0, 15                              ;   xm.vgetc t3}
    {   xm.zexti t3, 5                             ;   lw s8, 4                          (sp)}
    {   sub a0, a0, t3                         ;   xm.retsp (NSTACKWORDS)*4                       }


.L_func_end:

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__VX4B__)



