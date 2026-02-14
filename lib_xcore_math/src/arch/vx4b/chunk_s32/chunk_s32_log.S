// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  

Condition:  0 < ldexp(b[k], -30) < 2


void chunk_s32_log(
    q8_24 a[],
    const int32_t b[],
    const exponent_t b_exp);
*/


#define NSTACKWORDS     (8+8*6)

#define FUNCTION_NAME   chunk_s32_log

#define SP_VEC_X1    ((NSTACKWORDS) - 8 -1)
#define SP_VEC_X2    ((NSTACKWORDS) - 16-1)
#define SP_VEC_X3    ((NSTACKWORDS) - 24-1)
#define SP_VEC_X4    ((NSTACKWORDS) - 32-1)
#define SP_VEC_X5    ((NSTACKWORDS) - 40-1)
#define SP_VEC_X6    ((NSTACKWORDS) - 48-1)


.text
.p2align 2

.L_ps_coef1: .word -0x800000, -0x800000, -0x800000, -0x800000, -0x800000, -0x800000, -0x800000, -0x800000
.L_ps_coef2: .word  0x555555,  0x555555,  0x555555,  0x555555,  0x555555,  0x555555,  0x555555,  0x555555
.L_ps_coef3: .word -0x400000, -0x400000, -0x400000, -0x400000, -0x400000, -0x400000, -0x400000, -0x400000
.L_ps_coef4: .word  0x333333,  0x333333,  0x333333,  0x333333,  0x333333,  0x333333,  0x333333,  0x333333
.L_ps_coef5: .word -0x2aaaab, -0x2aaaab, -0x2aaaab, -0x2aaaab, -0x2aaaab, -0x2aaaab, -0x2aaaab, -0x2aaaab
 
.L_ln_2: .word 0x2c5c85fe,0x2c5c85fe,0x2c5c85fe,0x2c5c85fe,0x2c5c85fe,0x2c5c85fe,0x2c5c85fe,0x2c5c85fe

#define a           a0 
#define b           a1
#define b_exp       a2
#define mantB       a3
#define tmpA        s2
#define tmpB        s3
#define tmpC        s4
#define vec_x       s5
#define mantA       t3

FUNCTION_NAME:
xm.entsp (NSTACKWORDS)*4
xm.stdsp  s3,s2,0
xm.stdsp  s5,s4,8

{ addi vec_x,sp, (SP_VEC_X1)*4          ; li t3, 0                              }
{ nop                                   ; xm.vsetc t3                           }
xm.lddi  mantA,mantB, 0(b)
{ xm.cls tmpA, mantA                    ; nop                                   }
{ nop                                   ; xm.cls tmpB, mantB                    }
{ xm.shl mantA, mantA, tmpA             ; xm.shl mantB, mantB, tmpB             }
xm.stdi  mantA,mantB, 0(vec_x)
{ sub mantA, b_exp, tmpA                ; sub mantB, b_exp, tmpB                }
xm.stdi  mantA,mantB, 0(a)

xm.lddi  mantA,mantB, 8(b)
{ xm.cls tmpA, mantA                    ; nop                                   }
{ nop                                   ; xm.cls tmpB, mantB                    }
{ xm.shl mantA, mantA, tmpA             ; xm.shl mantB, mantB, tmpB             }
xm.stdi  mantA,mantB, 8(vec_x)
{ sub mantA, b_exp, tmpA                ; sub mantB, b_exp, tmpB                }
xm.stdi  mantA,mantB, 8(a)

xm.lddi  mantA,mantB, 16(b)
{ xm.cls tmpA, mantA                    ; nop                                   }
{ nop                                   ; xm.cls tmpB, mantB                    }
{ xm.shl mantA, mantA, tmpA             ; xm.shl mantB, mantB, tmpB             }
xm.stdi  mantA,mantB, 16(vec_x)
{ sub mantA, b_exp, tmpA                ; sub mantB, b_exp, tmpB                }
xm.stdi  mantA,mantB, 16(a)

xm.lddi  mantA,mantB, 24(b)
{ xm.cls tmpA, mantA                    ; nop                                   }
{ nop                                   ; xm.cls tmpB, mantB                    }
{ xm.shl mantA, mantA, tmpA             ; xm.shl mantB, mantB, tmpB             }
xm.stdi  mantA,mantB, 24(vec_x)
{ sub mantA, b_exp, tmpA                ; sub mantB, b_exp, tmpB                }
xm.stdi  mantA,mantB, 24(a)

{ li tmpA, 24                           ; nop                                   }

la t3, vpu_vec_0x20000000
{ nop                                   ; xm.vclrdr                             }
{ xm.neg tmpA, tmpA                     ; nop                                   }
xm.vlashr a, tmpA
{ xm.ldap t3, .L_ln_2                   ; xm.vladd t3                           }
{ nop                                   ; xm.vlmul0 t3                          }
{ mv t3, vec_x                          ; xm.vstr a                             }
{ nop                                   ; xm.vldr t3                            }
la t3, vpu_vec_0x00000002
xm.vlsat t3
la t3, vpu_vec_neg_0x40000000
{ addi tmpB,sp, (SP_VEC_X1)*4           ; xm.vladd t3                           }

#undef mantA
#undef mantB

{ addi vec_x,sp, (SP_VEC_X2)*4          ; xm.vstr vec_x                         }
{ nop                                   ; xm.vlmul0 tmpB                        } // (x-1.0)^2
{ addi vec_x,sp, (SP_VEC_X3)*4          ; xm.vstr vec_x                         }
{ nop                                   ; xm.vlmul0 tmpB                        } // (x-1.0)^3
{ addi vec_x,sp, (SP_VEC_X4)*4          ; xm.vstr vec_x                         }
{ nop                                   ; xm.vlmul0 tmpB                        } // (x-1.0)^4
{ addi vec_x,sp, (SP_VEC_X5)*4          ; xm.vstr vec_x                         }
{ li tmpA, 6                            ; xm.vlmul0 tmpB                        } // (x-1.0)^5
{ addi vec_x,sp, (SP_VEC_X6)*4          ; xm.vstr vec_x                         }
{ xm.ldap t3, .L_ps_coef5               ; xm.vlmul0 tmpB                        } // (x-1.0)^6
{ addi tmpB,sp, (SP_VEC_X1)*4           ; xm.vstr vec_x                         }

  xm.vlashr tmpB, tmpA                                         // vR[] = coef[0] * x
{ xm.ldap t3, .L_ps_coef4               ; xm.vldc t3                            } // vC[] = coef[5]
{ addi vec_x,sp, (SP_VEC_X5)*4          ; xm.vlmacc0 vec_x                      } // vR[] += coef[5] * x^6
{ xm.ldap t3, .L_ps_coef3               ; xm.vldc t3                            } // vC[] = coef[4]
{ addi vec_x,sp, (SP_VEC_X4)*4          ; xm.vlmacc0 vec_x                      } // vR[] += coef[4] * x^5
{ xm.ldap t3, .L_ps_coef2               ; xm.vldc t3                            } // vC[] = coef[3]
{ addi vec_x,sp, (SP_VEC_X3)*4          ; xm.vlmacc0 vec_x                      } // vR[] += coef[3] * x^4
{ xm.ldap t3, .L_ps_coef1               ; xm.vldc t3                            } // vC[] = coef[2]
{ addi vec_x,sp, (SP_VEC_X2)*4          ; xm.vlmacc0 vec_x                      } // vR[] += coef[2] * x^3
{ nop                                   ; xm.vldc t3                            } // vC[] = coef[1]
{ addi vec_x,sp, (SP_VEC_X1)*4          ; xm.vlmacc0 vec_x                      } // vR[] += coef[1] * x^2

{ nop                                   ; xm.vladd a                            }
{ nop                                   ; xm.vstr a                             }

// Any inputs that were 0 should become INT32_MIN
  la t3, vpu_vec_0x00000001
{ nop                                   ; xm.vldr t3                            } 
{ nop                                   ; xm.vlsub b                            } 
{ nop                                   ; xm.vdepth1                            } 
{ nop                                   ; xm.vstr vec_x                         }
{ nop                                   ; lw tmpA,0          ( vec_x)           }
{ mv tmpB, tmpA                         ; nop                                   }
xm.zip tmpB, tmpA, 0
mv tmpB, tmpA
xm.zip tmpB, tmpA, 0
la t3, vpu_vec_0x80000000
{ nop                                   ; xm.vldr t3                            }
xm.vstrpv a, tmpA

.L_finish:
xm.lddsp  s3,s2,0
xm.lddsp  s5,s4,8
xm.retsp (NSTACKWORDS)*4

.L_func_end:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME

#endif //defined(__VX4B__)



