// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  

headroom_t vect_s16_clip( 
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const int16_t lower_bound,
    const int16_t upper_bound,
    const int b_shr);
*/


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (6)
#define NSTACKWORDS     (8 + 8*(NSTACKVECS)+4)

#define FUNCTION_NAME   vect_s16_clip

#define STACK_VEC(K)    (NSTACKWORDS - (8*((K)+1))-4)

#define a           a0
#define b           a1
#define N           a2
#define lower       a3
#define upper       x18
#define b_shr       x19
#define tail        s4
#define tmp1        s5
#define tmp2        x22
#define int_max     s7
#define int_min     s8

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
FUNCTION_NAME:

        xm.entsp (NSTACKWORDS)*4
        xm.stdsp  s3,s2,0
        xm.stdsp  s5,s4,8
        li t3, 0x0100
        xm.stdsp  s7,s6,16
    { slli tail, N, SIZEOF_LOG2_S16     ; xm.vsetc t3                           }
    { xm.zexti tail, 5                  ; sw s8, 24                          (sp) }

    { li tmp1, 15                       ; srli N, N, EPV_LOG2_S16               }
    { xm.mkmsk int_max, tmp1            ; xm.vclrdr                             }   
    { xm.addi int_min, int_max, 1       ; xm.mkmsk tail, tail                   }

    // If upper >= 0  and lower <= 0, we can do this more efficiently.
    mv upper, a4
    { li tmp1, 0                        ; nop                                   }
    mv b_shr, a5
    { xm.slt tmp2, upper, tmp1          ; nop                                   }
    { xm.slt tmp1, tmp1, lower          ; nop                                   }
    bnez tmp2, .L_lower_nice 
    bnez tmp1, .L_upper_nice 


    // Otherwise, we have the nice situation.
.L_nice:

    //In the nice situation, the upper bound is no more than 1 VLADD away from the positive  saturation 
    //  point of the VPU, and the lower bound is no more than 1 VLADD away from the negative saturation
    //  point of the VPU. 

    { addi t3,sp, (STACK_VEC(0))*4      ; sub upper, int_max, upper             }
    { mv tmp1, upper                    ; mv tmp2, upper                        }
xm.zip tmp2, tmp1, 4
    { nop                               ; xm.bl .L_std_func1                    }

    { addi t3,sp, (STACK_VEC(2))*4      ; xm.neg upper, upper                   }
    { mv tmp1, upper                    ; mv tmp2, upper                        }
xm.zip tmp2, tmp1, 4
    { nop                               ; xm.bl .L_std_func1                    }

    { addi t3,sp, (STACK_VEC(1))*4      ; sub lower, int_min, lower             }
    { mv tmp1, lower                    ; mv tmp2, lower                        }
xm.zip tmp2, tmp1, 4
    { nop                               ; xm.bl .L_std_func1                    }

    { addi t3,sp, (STACK_VEC(3))*4      ; xm.neg lower, lower                   }
    { mv tmp1, lower                    ; mv tmp2, lower                        }
xm.zip tmp2, tmp1, 4
    { nop                               ; xm.bl .L_std_func1                    }

    { nop                               ; xm.bu .L_std_func_end1                }
.L_std_func1:
        xm.stdi  tmp1,tmp1, 0(t3)
        xm.stdi  tmp1,tmp1, 8(t3)
        xm.stdi  tmp1,tmp1, 16(t3)
        xm.stdi  tmp1,tmp1, 24(t3)
        ret 
.L_std_func_end1:

#define vec_upper   upper
#define vec_lower   lower
#define vec_nupper  tmp1
#define vec_nlower  tmp2
#define _32         int_min

//{   nop; xm.ldawsp vec_upper, STACK_VEC(0)*4        }
//{   nop; xm.ldawsp vec_lower, STACK_VEC(1)*4        }
//{   nop; xm.ldawsp vec_nupper, STACK_VEC(2)*4       }
//{   nop; xm.ldawsp vec_nlower, STACK_VEC(3)*4       }
{ addi vec_upper,sp, (STACK_VEC(0))*4   ; nop                                   }
{ addi vec_lower,sp, (STACK_VEC(1))*4   ; nop                                   }
{ addi vec_nupper,sp, (STACK_VEC(2))*4  ; nop                                   }
{ addi vec_nlower,sp, (STACK_VEC(3))*4  ; nop                                   }
    { li _32, 32                        ; xm.brff N, .L_nice_loop_bot           }

    .L_nice_loop_top:
            xm.vlashr b, b_shr
        { add b, b, _32                 ; xm.vladd vec_upper                    }
        { addi N, N, -1                 ; xm.vladd vec_nupper                   }
        { nop                           ; xm.vladd vec_lower                    }
        { nop                           ; xm.vladd vec_nlower                   }
        { add a, a, _32                 ; xm.vstr a                             }
        { nop                           ; xm.bt N, .L_nice_loop_top             }
    .L_nice_loop_bot:
    
     beqz tail, .L_finish          
        xm.vlashr b, b_shr
    { nop                               ; xm.vladd vec_upper                    }
    { nop                               ; xm.vladd vec_nupper                   }
    { nop                               ; xm.vladd vec_lower                    }
    { nop                               ; xm.vladd vec_nlower                   }
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

    { sub vec_one, int_max, upper       ; xm.neg vec_three, lower               }
     addi vec_three, vec_three, -1
    { addi vec_two, int_min, 1          ; xm.bu .L_not_nice_thing               }
    
.L_lower_nice:
    { sub vec_one, int_min, lower       ; xm.neg vec_three, upper               }
    { mv vec_two, int_max               ; nop                                   }


.L_not_nice_thing:

    { addi t3,sp, (STACK_VEC(0))*4      ; nop                                   }
    { mv s6, vec_one                    ; mv s7, vec_one                        }
xm.zip s7, s6, 4
    { nop                               ; xm.bl .L_std_func                     }

    { addi t3,sp, (STACK_VEC(1))*4      ; nop                                   }
    { mv s6, vec_two                    ; mv s7, vec_two                        }
xm.zip s7, s6, 4
    { nop                               ; xm.bl .L_std_func                     }

    { addi t3,sp, (STACK_VEC(3))*4      ; nop                                   }
{ xm.neg s6, vec_one                    ; nop                                   }
{ nop                                   ; xm.neg s7, vec_one                    }
xm.zip s7, s6, 4
    { nop                               ; xm.bl .L_std_func                     }

    { addi t3,sp, (STACK_VEC(2))*4      ; nop                                   }
{ xm.neg s6, vec_two                    ; nop                                   }
{ nop                                   ; xm.neg s7, vec_two                    }
xm.zip s7, s6, 4
    { nop                               ; xm.bl .L_std_func                     }

    { addi t3,sp, (STACK_VEC(4))*4      ; nop                                   }
    { mv s6, vec_three                  ; mv s7, vec_three                      }
xm.zip s7, s6, 4
    { nop                               ; xm.bl .L_std_func                     }

    { addi t3,sp, (STACK_VEC(5))*4      ; nop                                   }
{ xm.neg s6, vec_three                  ; nop                                   }
{ nop                                   ; xm.neg s7, vec_three                  }
xm.zip s7, s6, 4
    { nop                               ; xm.bl .L_std_func                     }

    { nop                               ; xm.bu .L_std_func_end                 }
.L_std_func:
        xm.stdi  s6,s6, 0(t3)
        xm.stdi  s6,s6, 8(t3)
        xm.stdi  s6,s6, 16(t3)
        xm.stdi  s6,s6, 24(t3)
        ret 
.L_std_func_end:
/*
{ nop                                   ; xm.ldawsp vec_one, STACK_VEC(0)  *4   }
{ nop                                   ; xm.ldawsp vec_none, STACK_VEC(3) *4   }
{ nop                                   ; xm.ldawsp vec_two, STACK_VEC(1)  *4   }
{ nop                                   ; xm.ldawsp vec_ntwo, STACK_VEC(2) *4   }
{ nop                                   ; xm.ldawsp vec_three, STACK_VEC(4) *4  }
{ nop                                   ; xm.ldawsp vec_nthree, STACK_VEC(5)*4  }
*/
{ addi vec_one,sp, (STACK_VEC(0))*4     ; nop                                   }
{ addi vec_none,sp, (STACK_VEC(3))*4    ; nop                                   }
{ addi vec_two,sp, (STACK_VEC(1))*4     ; nop                                   }
{ addi vec_ntwo,sp, (STACK_VEC(2))*4    ; nop                                   }
{ addi vec_three,sp, (STACK_VEC(4))*4   ; nop                                   }
{ addi vec_nthree,sp, (STACK_VEC(5))*4  ; nop                                   }

    { li t3, 32                         ; xm.brff N, .L_not_nice_loop_bot       }
.L_not_nice_loop_top:
            xm.vlashr b, b_shr
        { add b, b, t3                  ; xm.vladd vec_one                      }
        { addi N, N, -1                 ; xm.vladd vec_none                     }
        { nop                           ; xm.vladd vec_two                      }
        { nop                           ; xm.vladd vec_three                    }
        { nop                           ; xm.vladd vec_nthree                   }
        { nop                           ; xm.vladd vec_ntwo                     }
        { add a, a, t3                  ; xm.vstr a                             }
        { nop                           ; xm.bt N, .L_not_nice_loop_top         }
.L_not_nice_loop_bot:
    
    { nop                               ; xm.brff tail, .L_finish               }
        xm.vlashr b, b_shr
    { nop                               ; xm.vladd vec_one                      }
    { nop                               ; xm.vladd vec_none                     }
    { nop                               ; xm.vladd vec_two                      }
    { nop                               ; xm.vladd vec_three                    }
    { nop                               ; xm.vladd vec_nthree                   }
    { nop                               ; xm.vladd vec_ntwo                     }


.L_finishish:
    { nop                               ; xm.vstd tmp1                          }
    xm.vstrpv a, tail
    xm.vstrpv tmp1, tail
    { nop                               ; xm.vldd tmp1                          }
    { nop                               ; xm.vstd tmp1                          }

.L_finish:
    { li a0, 15                         ; xm.vgetc t3                           }
    { xm.zexti t3, 5                    ; lw s8, 24                          (sp) }
        xm.lddsp  s3,s2,0
        xm.lddsp  s5,s4,8
        xm.lddsp  s7,s6,16
    { sub a0, a0, t3                    ; xm.retsp (NSTACKWORDS)*4              } 

.L_func_end:

//.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME





#endif //defined(__VX4B__)
