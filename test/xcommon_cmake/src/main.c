
#include "xmath/xmath.h"

int main(void)
{
  // Just reference a few symbols to make sure that they can be linked in
  headroom_t (*ptr0) (int32_t *, const int32_t*, const int32_t*, const unsigned, const right_shift_t, const right_shift_t);
  ptr0 = &vect_s32_add;
  printf("prt0 = %lx\n", (long)ptr0);

  float (*ptr1) (const float*, const float*, const unsigned);
  ptr1 = &vect_f32_dot;
  printf("prt1 = %lx\n", (long)ptr1);

  bfp_complex_s32_t *(*ptr2) (bfp_s32_t*);
  ptr2 = &bfp_fft_forward_mono;
  printf("prt2 = %lx\n", (long)ptr2);

  q8_24 (*ptr3) (const q8_24);
  ptr3 = &q24_logistic_fast;
  printf("prt3 = %lx\n", (long)ptr3);

  int32_t (*ptr4) (filter_biquad_s32_t*, const int32_t);
  ptr4 = &filter_biquad_s32;
  printf("prt4 = %lx\n", (long)ptr4);

  float_s32_t (*ptr5) (const double);
  ptr5 = &f64_to_float_s32;
  printf("prt5 = %lx\n", (long)ptr5);

  return 0;
}
