// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


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
.issue_mode dual
.align 4

#define NSTACKVECS      (6)
#define NSTACKWORDS     (8 + 8*(NSTACKVECS))

#define FUNCTION_NAME   vect_s16_clip


#define STACK_UPPER     (NSTACKWORDS + 1)
#define STACK_B_SHR     (NSTACKWORDS + 2)

#define STACK_VEC(K)    (NSTACKWORDS - (8*((K)+1)))

#define a           r0
#define b           r1
#define N           r2
#define lower       r3
#define upper       r4
#define b_shr       r5
#define tail        r6
#define tmp1        r7
#define tmp2        r8
#define int_max     r9
#define int_min     r10

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:

        dualentsp NSTACKWORDS
        std r4, r5, sp[0]
        std r6, r7, sp[1]
        ldc r11, 0x0100
        std r8, r9, sp[2]
    {   shl tail, N, SIZEOF_LOG2_S16            ;   vsetc r11                               }
    {   zext tail, 5                            ;   stw r10, sp[6]                          }

    {   ldc tmp1, 15                            ;   shr N, N, EPV_LOG2_S16                  }
    {   mkmsk int_max, tmp1                     ;   vclrdr                                  }   
    {   neg int_min, int_max                    ;   mkmsk tail, tail                        }

    // If upper >= 0  and lower <= 0, we can do this more efficiently.
    {   ldc tmp1, 0                             ;   ldw upper, sp[STACK_UPPER]              }
    {   lss tmp2, upper, tmp1                   ;   ldw b_shr, sp[STACK_B_SHR]              }
    {   lss tmp1, tmp1, lower                   ;   bt tmp2, .L_lower_nice                  }
    {                                           ;   bt tmp1, .L_upper_nice                  }


    // Otherwise, we have the nice situation.
.L_nice:

    //In the nice situation, the upper bound is no more than 1 VLADD away from the positive  saturation 
    //  point of the VPU, and the lower bound is no more than 1 VLADD away from the negative saturation
    //  point of the VPU. 

    {   ldaw r11, sp[STACK_VEC(0)]              ;   sub upper, int_max, upper               }
    {   mov tmp1, upper                         ;   mov tmp2, upper                         }
    zip tmp2, tmp1, 4
    {                                           ;   bl .L_std_func1                         }

    {   ldaw r11, sp[STACK_VEC(2)]              ;   neg upper, upper                        }
    {   mov tmp1, upper                         ;   mov tmp2, upper                         }
    zip tmp2, tmp1, 4
    {                                           ;   bl .L_std_func1                         }

    {   ldaw r11, sp[STACK_VEC(1)]              ;   sub lower, int_min, lower               }
    {   mov tmp1, lower                         ;   mov tmp2, lower                         }
    zip tmp2, tmp1, 4
    {                                           ;   bl .L_std_func1                         }

    {   ldaw r11, sp[STACK_VEC(3)]              ;   neg lower, lower                        }
    {   mov tmp1, lower                         ;   mov tmp2, lower                         }
    zip tmp2, tmp1, 4
    {                                           ;   bl .L_std_func1                         }

    {                                           ;   bu .L_std_func_end1                     }
.L_std_func1:
        std tmp1, tmp1, r11[0]
        std tmp1, tmp1, r11[1]
        std tmp1, tmp1, r11[2]
        std tmp1, tmp1, r11[3]
        retsp 0
.L_std_func_end1:

#define vec_upper   upper
#define vec_lower   lower
#define vec_nupper  tmp1
#define vec_nlower  tmp2
#define _32         int_min

    {   ldaw vec_upper, sp[STACK_VEC(0)]        ;   ldaw vec_lower, sp[STACK_VEC(1)]        }
    {   ldaw vec_nupper, sp[STACK_VEC(2)]       ;   ldaw vec_nlower, sp[STACK_VEC(3)]       }
    {   ldc _32, 32                             ;   bf N, .L_nice_loop_bot                  }

    .L_nice_loop_top:
            vlashr b[0], b_shr
        {   add b, b, _32                       ;   vladd vec_upper[0]                      }
        {   sub N, N, 1                         ;   vladd vec_nupper[0]                     }
        {                                       ;   vladd vec_lower[0]                      }
        {                                       ;   vladd vec_nlower[0]                     }
        {   add a, a, _32                       ;   vstr a[0]                               }
        {                                       ;   bt N, .L_nice_loop_top                  }
    .L_nice_loop_bot:
    
    {                                       ;   bf tail, .L_finish                      }
        vlashr b[0], b_shr
    {                                       ;   vladd vec_upper[0]                      }
    {                                       ;   vladd vec_nupper[0]                     }
    {                                       ;   vladd vec_lower[0]                      }
    {                                       ;   vladd vec_nlower[0]                     }
    {                                       ;   bu .L_finishish                         }

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

    {   sub vec_one, int_max, upper             ;   neg vec_three, lower                    }
    {   mov vec_two, int_min                    ;   bu .L_not_nice_thing                    }
    
.L_lower_nice:
    {   sub vec_one, int_min, lower             ;   neg vec_three, upper                    }
    {   mov vec_two, int_max                    ;                                           }


.L_not_nice_thing:

    {   ldaw r11, sp[STACK_VEC(0)]              ;                                           }
    {   mov r8, vec_one                         ;   mov r9, vec_one                         }
        zip r9, r8, 4
    {                                           ;   bl .L_std_func                          }

    {   ldaw r11, sp[STACK_VEC(1)]              ;                                           }
    {   mov r8, vec_two                         ;   mov r9, vec_two                         }
        zip r9, r8, 4
    {                                           ;   bl .L_std_func                          }

    {   ldaw r11, sp[STACK_VEC(3)]              ;                                           }
    {   neg r8, vec_one                         ;   neg r9, vec_one                         }
        zip r9, r8, 4
    {                                           ;   bl .L_std_func                          }

    {   ldaw r11, sp[STACK_VEC(2)]              ;                                           }
    {   neg r8, vec_two                         ;   neg r9, vec_two                         }
        zip r9, r8, 4
    {                                           ;   bl .L_std_func                          }

    {   ldaw r11, sp[STACK_VEC(4)]              ;                                           }
    {   mov r8, vec_three                       ;   mov r9, vec_three                       }
        zip r9, r8, 4
    {                                           ;   bl .L_std_func                          }

    {   ldaw r11, sp[STACK_VEC(5)]              ;                                           }
    {   neg r8, vec_three                       ;   neg r9, vec_three                       }
        zip r9, r8, 4   
    {                                           ;   bl .L_std_func                          }

    {                                           ;   bu .L_std_func_end                      }
.L_std_func:
        std r8, r8, r11[0]
        std r8, r8, r11[1]
        std r8, r8, r11[2]
        std r8, r8, r11[3]
        retsp 0
.L_std_func_end:

    {   ldaw vec_one, sp[STACK_VEC(0)]          ;   ldaw vec_none, sp[STACK_VEC(3)]         }
    {   ldaw vec_two, sp[STACK_VEC(1)]          ;   ldaw vec_ntwo, sp[STACK_VEC(2)]         }
    {   ldaw vec_three, sp[STACK_VEC(4)]        ;   ldaw vec_nthree, sp[STACK_VEC(5)]       }

    {   ldc r11, 32                             ;   bf N, .L_not_nice_loop_bot              }
.L_not_nice_loop_top:
            vlashr b[0], b_shr
        {   add b, b, r11                       ;   vladd vec_one[0]                        }
        {   sub N, N, 1                         ;   vladd vec_none[0]                       }
        {                                       ;   vladd vec_two[0]                        }
        {                                       ;   vladd vec_three[0]                      }
        {                                       ;   vladd vec_nthree[0]                     }
        {                                       ;   vladd vec_ntwo[0]                       }
        {   add a, a, r11                       ;   vstr a[0]                               }
        {                                       ;   bt N, .L_not_nice_loop_top              }
.L_not_nice_loop_bot:
    
    {                                       ;   bf tail, .L_finish                      }
        vlashr b[0], b_shr
    {                                       ;   vladd vec_one[0]                        }
    {                                       ;   vladd vec_none[0]                       }
    {                                       ;   vladd vec_two[0]                        }
    {                                       ;   vladd vec_three[0]                      }
    {                                       ;   vladd vec_nthree[0]                     }
    {                                       ;   vladd vec_ntwo[0]                       }


.L_finishish:
    {                                       ;   vstd tmp1[0]   /*save zeros*/           }
    vstrpv a[0], tail
    vstrpv tmp1[0], tail
    {                                       ;   vldd tmp1[0]                            }
    {                                       ;   vstd tmp1[0]                            }

.L_finish:
    {   ldc r0, 15                              ;   vgetc r11                               }
    {   zext r11, 5                             ;   ldw r10, sp[6]                          }
        ldd r4, r5, sp[0]
        ldd r6, r7, sp[1]
        ldd r8, r9, sp[2]
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                       } 

.L_func_end:

.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME





#endif //defined(__XS3A__)