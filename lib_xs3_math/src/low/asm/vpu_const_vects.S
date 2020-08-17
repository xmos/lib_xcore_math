

//Note that some of these vecors overlap one another. No point in wasting memory.

#define VPU_VEC(NAME)   .global NAME; .type NAME,@object; .size NAME,32; NAME

#define REPEAT_C(R,I)   R,I,R,I,R,I,R,I
#define REPEAT8(V)      V,V,V,V,V,V,V,V
#define REPEAT16(V)     REPEAT8(V),REPEAT8(V)
#define REPEAT32(V)     REPEAT16(V),REPEAT16(V)


.pushsection .cp.vpu_consts, "ac", @progbits
.align 4

.cc_top vpu_vec_zero
    VPU_VEC(vpu_vec_zero):
        .space 32, 0
.cc_bottom vpu_vec_zero
.cc_top vpu_vec_neg_1
    VPU_VEC(vpu_vec_neg_1):
        .space 32, 0xFF
.cc_bottom vpu_vec_neg_1

.pushsection .cp.vpu_consts.cplx, "ac", @progbits

    .cc_top vpu_consts_complex

        VPU_VEC(vpu_vec_complex_pos_j):
            .word 0x00000000

        VPU_VEC(vpu_vec_complex_ones):
            .word  REPEAT_C(0x40000000, 0x00000000)

        VPU_VEC(vpu_vec_complex_conj_op):
            .word REPEAT_C(0x40000000, -0x40000000)

        VPU_VEC(vpu_vec_complex_neg_j):
            .word 0x00000000

        VPU_VEC(vpu_vec_complex_neg_ones):
            .word REPEAT_C(-0x40000000, 0)
    .cc_bottom vpu_consts_complex
.popsection


.pushsection .cp.vpu_consts.32, "ac", @progbits
    .cc_top    vpu_vec_ones_s32
        VPU_VEC(vpu_vec_ones_s32):
            .word REPEAT8(0x40000000)
    .cc_bottom vpu_vec_ones_s32
    .cc_top    vpu_vec_neg_ones_s32
        VPU_VEC(vpu_vec_neg_ones_s32):
            .word REPEAT8(-0x40000000)
    .cc_bottom vpu_vec_neg_ones_s32
.popsection

.pushsection .cp.vpu_consts.16, "ac", @progbits
    .cc_top    vpu_vec_0x0001
        VPU_VEC(vpu_vec_0x0001):
            .short REPEAT16(1)
    .cc_bottom vpu_vec_0x0001
    .cc_top    vpu_vec_0x4000
        VPU_VEC(vpu_vec_0x4000):
            .short REPEAT16(0x4000)
    .cc_bottom vpu_vec_0x4000

    .cc_top    vpu_vec_neg_0x4000
        VPU_VEC(vpu_vec_neg_0x4000):
            .short REPEAT16(-0x4000)
    .cc_bottom vpu_vec_neg_0x4000
.popsection

.pushsection .cp.vpu_consts.8, "ac", @progbits
    .cc_top    vpu_vec_0x01
        VPU_VEC(vpu_vec_0x01):
            .byte REPEAT32(1)
    .cc_bottom vpu_vec_0x01
    .cc_top    vpu_vec_0x40
        VPU_VEC(vpu_vec_0x40):
            .byte REPEAT32(0x40)
    .cc_bottom vpu_vec_0x40
.popsection