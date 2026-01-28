// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  

headroom_t vect_s32_clip(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const int32_t lower_bound,
    const int32_t upper_bound,
    const int b_shr);
*/


#include "../asm_helper.h"

.text
.p2align 2


#define NSTACKVECS      (6)
#define NSTACKWORDS     (8 + 8*(NSTACKVECS)+8)

#define FUNCTION_NAME   vect_s32_clip

#define STACK_VEC(K)    (NSTACKWORDS - (8*((K)+1))-8)

#define a           x10
#define b           x11
#define N           x12
#define lower       x13
#define upper       x18
#define b_shr       x19
#define tail        x20
#define tmp1        x21
#define tmp2        x22
#define int_max     x23
#define int_min     x24


.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        xm.stdsp  s3,s2,0
        xm.stdsp  s5,s4,8
        li t3, 0x0
        xm.stdsp  s7,s6,16
    {   slli tail, N, SIZEOF_LOG2_S32            ;   xm.vsetc t3}/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli tail, N, SIZEOF_LOG2_S32            \nMessage: The shift amount is not 32" */
    {   xm.zexti tail, 5                            ;   sw s8, 24                          (sp)}

    {   li tmp1, 31                            ;   srli N, N, EPV_LOG2_S32                  }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri N, N, EPV_LOG2_S32                  \nMessage: The shift amount is not 32" */
    {   xm.mkmsk int_max, tmp1                     ;   xm.vclrdr    
                                  }  
    {   xm.addi int_min, int_max, 1                ;   xm.mkmsk tail, tail                        }

    // If upper >= 0  and lower <= 0, we can do this more efficiently.
    mv upper, a4
    {   li tmp1, 0                             ;   nop}
    mv b_shr, a5
    {   xm.slt tmp2, upper, tmp1                   ;   nop  }
        bnez tmp2, .L_lower_nice 
    {   xm.slt tmp1, tmp1, lower                   ;   nop  }
        bnez tmp1, .L_upper_nice 


    // Otherwise, we have the nice situation.
.L_nice:

    //In the nice situation, the upper bound is no more than 1 VLADD away from the positive  saturation 
    //  point of the VPU, and the lower bound is no more than 1 VLADD away from the negative saturation
    //  point of the VPU. 

    {   addi t3,sp, (STACK_VEC(0))*4              ;   sub upper, int_max, upper               }
    { nop                                         ;   sub lower, int_min, lower               }


    xm.stdi  upper,upper, 0(t3)
    xm.stdi  upper,upper, 8(t3)
    xm.stdi  upper,upper, 16(t3)
    xm.stdi  upper,upper, 24(t3)

    { nop                                           ;   xm.neg upper, upper                        }
    { addi t3,sp, (STACK_VEC(1))*4                  ;   nop                                           }
    xm.stdi  lower,lower, 0(t3)
    xm.stdi  lower,lower, 8(t3)
    xm.stdi  lower,lower, 16(t3)
    xm.stdi  lower,lower, 24(t3)

    { nop                                           ;   xm.neg lower, lower                        }
    { nop                                           ;   addi t3,sp, (STACK_VEC(2))*4              }
    
    xm.stdi  upper,upper, 0(t3)
    xm.stdi  upper,upper, 8(t3)
    xm.stdi  upper,upper, 16(t3)
    xm.stdi  upper,upper, 24(t3)

    { nop                                           ;   addi t3,sp, (STACK_VEC(3))*4              }

    xm.stdi  lower,lower, 0(t3)
    xm.stdi  lower,lower, 8(t3)
    xm.stdi  lower,lower, 16(t3)
    xm.stdi  lower,lower, 24(t3)

#define vec_upper   upper
#define vec_lower   lower
#define vec_nupper  tmp1
#define vec_nlower  tmp2
#define _32         int_min

    {addi vec_upper,sp, (STACK_VEC(0))*4 ; nop}
    {addi vec_lower,sp, (STACK_VEC(1))*4 ; nop}
    {addi vec_nupper,sp, (STACK_VEC(2))*4 ; nop}
    {addi vec_nlower,sp, (STACK_VEC(3))*4 ; nop}
    {   li _32, 32                             ;   xm.brff N, .L_nice_loop_bot                  }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

    .L_nice_loop_top:
            xm.vlashr b, b_shr
        {   add b, b, _32                       ;   xm.vladd vec_upper}
        {   addi N, N, -1                       ;   xm.vladd vec_nupper}
        { nop                                   ;   xm.vladd vec_lower}
        { nop                                   ;   xm.vladd vec_nlower}
        {   add a, a, _32                       ;   xm.vstr a}
        { nop                                   ;   xm.bt N, .L_nice_loop_top                  }
    .L_nice_loop_bot:
    
      beqz tail, .L_finish                  
    
      xm.vlashr b, b_shr
    { nop                                       ;   xm.vladd vec_upper}
    { nop                                       ;   xm.vladd vec_nupper}
    { nop                                       ;   xm.vladd vec_lower}
    { nop                                       ;   xm.vladd vec_nlower}
     j .L_finishish 

/*
    C logic:

    void clip16(int16_t output[], int16_t input[], int16_t lower, int16_t upper, unsigned length, int input_shr)
    {
        if(upper >= 0 && lower <= 0){

            int16_t up_thing = VPU_INT16_MAX - upper;
            int16_t lo_thing = VPU_INT16_MIN - lower;

            // 7 instructions required
            for(unsigned int i = 0; i < length; i++){

                int16_t tmp = input[i] >> input_shr;
                tmp = SATURATING_ADD(tmp, up_thing);
                tmp = tmp - up_thing;
                tmp = SATURATING_ADD(tmp, lo_thing);
                tmp = tmp - lo_thing

                output[i] = tmp;
            }
        } else {

            int16_t one, two, three;

            if(upper >= 0){
                one = VPU_INT16_MAX - upper;
                two = VPU_INT16_MIN;
                three = VPU_INT16_MIN - (lower - upper);
            } else {
                one = VPU_INT16_MIN - lower;
                two = VPU_INT16_MAX;
                three = VPU_INT16_MAX - (upper - lower);
            }

            // 9 instructions required
            for(unsigned int i = 0; i < length; i++){

                int16_t tmp = input[i] >> input_shr;
                tmp = SATURATING_ADD(tmp, one);
                tmp = tmp - one;
                tmp = tmp + two;
                tmp = SATURATING_ADD(tmp, three);
                tmp = tmp - three;
                tmp = tmp - two;

                output[i] = tmp;
            }
        }
    }

*/



#undef vec_upper 
#undef vec_lower 
#undef vec_nupper
#undef vec_nlower
#undef _32       

#define vec_one     upper
#define vec_two     lower
#define vec_three   tmp1

#define vec_none    tmp2
#define vec_ntwo    int_max
#define vec_nthree  int_min

    // The nice thing about the not nice scenario is that at least one of the two bounds is
    //  guaranteed to be within one VLADD of the relevant saturation point.

.L_upper_nice:

    {   sub vec_one, int_max, upper             ;   xm.neg vec_three, lower                    }
    addi vec_three, vec_three, -1
    {   addi vec_two, int_min, 1                    ;   xm.bu .L_not_nice_thing                    }
    
.L_lower_nice:
    {   sub vec_one, int_min, lower             ;   xm.neg vec_three, upper                    }
    {   mv vec_two, int_max                    ; nop                                           }


.L_not_nice_thing:

    {   addi t3,sp, (STACK_VEC(0))*4              ; nop                                           }
        xm.stdi  vec_one,vec_one, 0(t3)
        xm.stdi  vec_one,vec_one, 8(t3)
        xm.stdi  vec_one,vec_one, 16(t3)
        xm.stdi  vec_one,vec_one, 24(t3)
    
    {   addi t3,sp, (STACK_VEC(1))*4              ; nop                                           }
        xm.stdi  vec_two,vec_two, 0(t3)
        xm.stdi  vec_two,vec_two, 8(t3)
        xm.stdi  vec_two,vec_two, 16(t3)
        xm.stdi  vec_two,vec_two, 24(t3)


 lui     t3, 0x80000
 bne t3, vec_one, .skip1
    addi vec_one, vec_one, 1
.skip1:
 bne t3, vec_two, .skip2
    addi vec_two, vec_two, 1
.skip2:
{   neg vec_one, vec_one                    ;  nop }
{   nop;neg vec_two, vec_two                    }

    {   addi t3,sp, (STACK_VEC(3))*4              ; nop                                           }
        xm.stdi  vec_one,vec_one, 0(t3)
        xm.stdi  vec_one,vec_one, 8(t3)
        xm.stdi  vec_one,vec_one, 16(t3)
        xm.stdi  vec_one,vec_one, 24(t3)
        
    {   addi t3,sp, (STACK_VEC(2))*4              ; nop                                           }
        xm.stdi  vec_two,vec_two, 0(t3)
        xm.stdi  vec_two,vec_two, 8(t3)
        xm.stdi  vec_two,vec_two, 16(t3)
        xm.stdi  vec_two,vec_two, 24(t3)

    {   xm.neg vec_two, vec_three                  ; nop                                           }
    {   addi t3,sp, (STACK_VEC(4))*4              ; nop                                           }
        xm.stdi  vec_three,vec_three, 0(t3)
        xm.stdi  vec_three,vec_three, 8(t3)
        xm.stdi  vec_three,vec_three, 16(t3)
        xm.stdi  vec_three,vec_three, 24(t3)
        
    {   addi t3,sp, (STACK_VEC(5))*4              ; nop                                           }
        xm.stdi  vec_two,vec_two, 0(t3)
        xm.stdi  vec_two,vec_two, 8(t3)
        xm.stdi  vec_two,vec_two, 16(t3)
        xm.stdi  vec_two,vec_two, 24(t3)

    {addi vec_one,sp, (STACK_VEC(0))*4 ; nop}
    {addi vec_none,sp, (STACK_VEC(3))*4 ; nop}
    {addi vec_two,sp, (STACK_VEC(1))*4 ; nop}
    {addi vec_ntwo,sp, (STACK_VEC(2))*4 ; nop}
    {addi vec_three,sp, (STACK_VEC(4))*4 ; nop}
    {addi vec_nthree,sp, (STACK_VEC(5))*4 ; nop}

    {   li t3, 32                             ;   xm.brff N, .L_not_nice_loop_bot              }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
.L_not_nice_loop_top:
            xm.vlashr b, b_shr
        { add b, b, t3                              ;   xm.vladd vec_one}
        { addi N, N, -1                             ;   xm.vladd vec_none}
        { nop                                       ;   xm.vladd vec_two}
        { nop                                       ;   xm.vladd vec_three}
        { nop                                       ;   xm.vladd vec_nthree}
        { nop                                       ;   xm.vladd vec_ntwo}
        { add a, a, t3                              ;   xm.vstr a}
        { nop                                       ;   xm.bt N, .L_not_nice_loop_top              }
.L_not_nice_loop_bot:
    
    { nop                                       ;   xm.brff tail, .L_finish                      }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
        xm.vlashr b, b_shr
    { nop                                       ;   xm.vladd vec_one}
    { nop                                       ;   xm.vladd vec_none}
    { nop                                       ;   xm.vladd vec_two}
    { nop                                       ;   xm.vladd vec_three}
    { nop                                       ;   xm.vladd vec_nthree}
    { nop                                       ;   xm.vladd vec_ntwo}


.L_finishish:
    { nop                                       ;   xm.vstd tmp1}
    xm.vstrpv a, tail
    xm.vstrpv tmp1, tail
    { nop                                       ;   xm.vldd tmp1}
    { nop                                       ;   xm.vstd tmp1}

.L_finish:
    {   li a0, 31                              ;   xm.vgetc t3}
    {   xm.zexti t3, 5                             ;   lw s8, 24                          (sp)}
        xm.lddsp  s3,s2,0
        xm.lddsp  s5,s4,8
        xm.lddsp  s7,s6,16
    {   sub a0, a0, t3                         ;   xm.retsp (NSTACKWORDS)*4                       } 

.L_func_end:

//.cc_bottom FUNCTION_NAME.function;  /* Translation error on this line: unexpected token at position 33. */ 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords;  /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores;  /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers;  /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends;  /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME




#endif //defined(__VX4B__)
