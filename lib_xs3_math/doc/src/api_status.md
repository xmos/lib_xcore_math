
API Function Status                 {#api_status}
===================

This table is unlikely to have actually captured everything that will be needed for an initial release (for example, it is currently unclear to me what low-level FFT functions are necessary). As new required API functions are identified they should be added to this table. Additionally, other functions (particularly utility functions) may have been written that haven't been included here.

### Low-Level Functions ###

| Function                                  | Implemented|| Tested | Documented |
| :---------------------------------------- | :--: | :--: | :----: | :--------: |
|                                           | ASM  |  C   |        |            | 
| @tab @tab xs3_bfp_init_low.h                                              ||||| 
| `xs3_set_vect_s16()`                      |  X   |  X   |   X    |     X      |
| `xs3_set_vect_s32()`                      |  X   |  X   |   X    |     X      |
| `xs3_set_vect_complex_s32()`              |  X   |  X   |   X    |     X      |
|  @tab @tab xs3_bfp_low.h                                                  ||||| 
| `xs3_headroom_vect_s16()`                 |  X   |  X   |   X    |     X      |
| `xs3_headroom_vect_s32()`                 |  X   |  X   |   X    |     X      |
| `xs3_shl_vect_s16()`                      |  X   |  X   |   X    |     X      |
| `xs3_shl_vect_s32()`                      |  X   |  X   |   X    |     X      |
| `xs3_add_vect_s16()`                      |  X   |  X   |   X    |     X      |
| `xs3_add_vect_s32()`                      |  X   |  X   |   X    |     X      |
| `xs3_sub_vect_s16()`                      |  X   |  X   |   X    |     X      |
| `xs3_sub_vect_s32()`                      |  X   |  X   |   X    |     X      |
| `xs3_mul_vect_s16()`                      |  X   |  X   |   X    |     X      |
| `xs3_mul_vect_s32()`                      |  X   |  X   |   X    |     X      |
| `xs3_scalar_mul_vect_s16()`               |  X   |  X   |   X    |     X      |
| `xs3_scalar_mul_vect_s32()`               |  X   |  X   |   X    |     X      |
| `xs3_abs_vect_s16()`                      |  X   |  X   |   X    |     X      |
| `xs3_abs_vect_s32()`                      |  X   |  X   |   X    |     X      |
| `xs3_sum_s16()`                           |  X   |  X   |   X    |     X      |
| `xs3_sum_s32()`                           |  X   |  X   |   X    |     X      |
| `xs3_dot_s16()`                           |  X   |  X   |   X    |     X      |
| `xs3_dot_s32()`                           |  X   |  X   |   X    |     X      |
| `xs3_clip_vect_s16()`                     |  X   |  X   |   X    |     X      |
| `xs3_clip_vect_s32()`                     |  X   |  X   |   X    |     X      |
| `xs3_rect_vect_s16()`                     |  X   |  X   |   X    |     X      |
| `xs3_rect_vect_s32()`                     |  X   |  X   |   X    |     X      |
| `xs3_s32_to_s16()`                        |  X   |  X   |   X    |     X      |
| `xs3_s16_to_s32()`                        |  X   |  X   |   X    |     X      |
|  @tab @tab xs3_bfp_complex_low.h                                          ||||| 
| `xs3_mul_vect_complex_s16()`              |  X   |  X   |   X    |     X      |
| `xs3_mul_vect_complex_s32()`              |  X   |  X   |   X    |     X      |
| `xs3_complex_mul_vect_complex_s16()`      |  X   |  X   |   X    |     X      |
| `xs3_complex_mul_vect_complex_s32()`      |  X   |  X   |   X    |     X      |
| `xs3_complex_conj_mul_vect_complex_s16()` |  X   |  X   |   X    |     X      |
| `xs3_complex_conj_mul_vect_complex_s32()` |  X   |  X   |   X    |     X      |
| `xs3_complex_scal_mul_vect_complex_s16()` |  X   |  X   |   X    |     X      |
| `xs3_complex_scal_mul_vect_complex_s32()` |  X   |  X   |   X    |     X      |
| `xs3_complex_s16_to_complex_s32()`        |  X   |  X   |   X    |     X      |
| `xs3_complex_s32_to_complex_s16()`        |  X   |  X   |   X    |     X      |
| `xs3_squared_mag_vect_complex_s16()`      |  X   |  X   |   X    |     X      |
| `xs3_squared_mag_vect_complex_s32()`      |  X   |  X   |   X    |     X      |
| `xs3_mag_vect_complex_s16()`              |  X   |  X   |   X    |            |
| `xs3_mag_vect_complex_s32()`              |  X   |  X   |   X    |            |
| `xs3_sum_complex_s16()`                   |  X   |  X   |   X    |     X      |
| `xs3_sum_complex_s32()`                   |  X   |  X   |   X    |     X      |
| `xs3_tail_reverse_complex_s32()`          |  X   |  X   |   X    |     X      |
|  @tab @tab xs3_fft.h                                                      ||||| 
| `xs3_fft_index_bit_reversal()`            |  X   |  X   |   X    |     X      |
| `xs3_fft_spectra_split()`                 |  X   |  X   |   X    |     X      |
| `xs3_fft_spectra_merge()`                 |  X   |  X   |   X    |     X      |
| `xs3_fft_mono_adjust()`                   |  X   |  X   |   X    |     X      |
| `xs3_fft_dit_forward()`                   |  X   |  X   |   X    |     X      |
| `xs3_fft_dit_inverse()`                   |  X   |  X   |   X    |     X      |
| `xs3_fft_dif_forward()`                   |  X   |  X   |   X    |     X      |
| `xs3_fft_dif_inverse()`                   |  X   |  X   |   X    |     X      |
|  @tab @tab xs3_filters_low.h                                              ||||| 
| `xs3_filters_fir()`                       |      |      |        |            |
| `xs3_filters_fir_add_sample()`            |      |      |        |            |
| `xs3_filters_interpolate()`               |      |      |        |            |
| `xs3_filters_decimate()`                  |      |      |        |            |
| `xs3_filters_biquad()`                    |      |      |        |            |
| `xs3_filters_biquads()`                   |      |      |        |            |
|  @tab @tab xs3_statistics_low.h                                           ||||| 
| `xs3_abs_sum_s16()`                       |      |      |        |            |
| `xs3_abs_sum_s32()`                       |      |      |        |            |
| `xs3_mean_s16()`                          |      |      |        |            |
| `xs3_mean_s32()`                          |      |      |        |            |
| `xs3_energy_s16()`                        |      |      |        |            |
| `xs3_energy_s32()`                        |      |      |        |            |
| `xs3_rms_s16()`                           |      |      |        |            |
| `xs3_rms_s32()`                           |      |      |        |            |
| `xs3_max_s16()`                           |      |      |        |            |
| `xs3_max_s32()`                           |      |      |        |            |
| `xs3_min_s16()`                           |      |      |        |            |
| `xs3_min_s32()`                           |      |      |        |            |
| `xs3_argmax_s16()`                        |      |      |        |            |
| `xs3_argmax_s32()`                        |      |      |        |            |
| `xs3_argmin_s16()`                        |      |      |        |            |
| `xs3_argmin_s32()`                        |      |      |        |            |


### High-Level API Functions ###

The high-level API functions are written entirely in C (perhaps C++ wrappers will eventually be made available), to implement block floating-point arithmetic which manage headroom and exponents for the uer. They use the low-level API to accomplish their work.

| Function                                  | Implemented | Tested | Documented |
| :---------------------------------------- | :---------: | :----: | :--------: |
| @tab @tab                  xs3_bfp_init.h                                  |||| 
| `bfp_init_vect_s16()`                     |      X      |   X    |     X      |
| `bfp_init_vect_s32()`                     |      X      |   X    |     X      |
| `bfp_init_vect_complex_s16()`             |      X      |   X    |     X      |
| `bfp_init_vect_complex_s32()`             |      X      |   X    |     X      |
| `bfp_init_vect_ch_pair_s16()`             |      X      |   X    |     X      |
| `bfp_init_vect_ch_pair_s32()`             |      X      |   X    |     X      |
| `bfp_set_vect_s16()`                      |      X      |   X    |     X      |
| `bfp_set_vect_s32()`                      |      X      |   X    |     X      |
| `bfp_set_vect_complex_s16()`              |      X      |   X    |     X      |
| `bfp_set_vect_complex_s32()`              |      X      |   X    |     X      |
| `bfp_set_vect_ch_pair_s16()`              |      X      |   X    |     X      |
| `bfp_set_vect_ch_pair_s32()`              |      X      |   X    |     X      |
| @tab @tab                       xs3_bfp.h                                  |||| 
| `bfp_headroom_vect_s16()`                 |      X      |   X    |     X      |
| `bfp_headroom_vect_s32()`                 |      X      |   X    |     X      |
| `bfp_shl_vect_s16()`                      |      X      |   X    |     X      |
| `bfp_shl_vect_s32()`                      |      X      |   X    |     X      |
| `bfp_add_vect_calc_params()`              |      X      |   X    |     X      |
| `bfp_add_vect_s16()`                      |      X      |   X    |     X      |
| `bfp_add_vect_s32()`                      |      X      |   X    |     X      |
| `bfp_sub_vect_calc_params()`              |      X      |   X    |     X      |
| `bfp_sub_vect_s16()`                      |      X      |   X    |     X      |
| `bfp_sub_vect_s32()`                      |      X      |   X    |     X      |
| `bfp_mul_vect_s16_calc_params()`          |      X      |   X    |     X      |
| `bfp_mul_vect_s16()`                      |      X      |   X    |     X      |
| `bfp_mul_vect_s32_calc_params()`          |      X      |   X    |     X      |
| `bfp_mul_vect_s32()`                      |      X      |   X    |     X      |
| `bfp_scalar_mul_vect_s16()`               |      X      |   X    |     X      |
| `bfp_scalar_mul_vect_s32()`               |      X      |   X    |     X      |
| `bfp_abs_vect_s16()`                      |      X      |   X    |     X      |
| `bfp_abs_vect_s32()`                      |      X      |   X    |     X      |
| `bfp_sum_s16()`                           |      X      |   X    |     X      |
| `bfp_sum_s32()`                           |      X      |   X    |     X      |
| `bfp_dot_s16()`                           |      X      |   X    |            |
| `bfp_dot_s32()`                           |      X      |   X    |            |
| `bfp_clip_vect_s16()`                     |      X      |   X    |            |
| `bfp_clip_vect_s32()`                     |      X      |   X    |            |
| `bfp_rect_vect_s16()`                     |      X      |   X    |     X      |
| `bfp_rect_vect_s32()`                     |      X      |   X    |     X      |
| `bfp_s32_to_s16()`                        |      X      |   X    |     X      |
| `bfp_s16_to_s32()`                        |      X      |   X    |     X      |
| @tab @tab               xs3_bfp_complex.h                                  |||| 
| `bfp_headroom_vect_complex_s16()`         |             |        |            |
| `bfp_headroom_vect_complex_s32()`         |             |        |            |
| `bfp_shl_vect_complex_s16()`              |             |        |            |
| `bfp_shl_vect_complex_s32()`              |             |        |            |
| `bfp_mul_vect_complex_s16()`              |             |        |            |
| `bfp_mul_vect_complex_s32()`              |             |        |            |
| `bfp_complex_mul_vect_complex_s16()`      |      X      |   X    |            |
| `bfp_complex_mul_vect_complex_s32()`      |      X      |   X    |            |
| `bfp_complex_conj_mul_vect_complex_s16()` |      X      |   X    |            |
| `bfp_complex_conj_mul_vect_complex_s32()` |      X      |   X    |            |
| `bfp_scalar_mul_vect_complex_s16()`       |             |        |            |
| `bfp_scalar_mul_vect_complex_s32()`       |             |        |            |
| `bfp_complex_scal_mul_vect_complex_s16()` |             |        |            |
| `bfp_complex_scal_mul_vect_complex_s32()` |             |        |            |
| `bfp_add_vect_complex_s16()`              |             |        |            |
| `bfp_add_vect_complex_s32()`              |             |        |            |
| `bfp_sub_vect_complex_s16()`              |             |        |            |
| `bfp_sub_vect_complex_s32()`              |             |        |            |
| `bfp_complex_s16_to_complex_s32()`        |             |        |            |
| `bfp_complex_s32_to_complex_s16()`        |             |        |            |
| `bfp_squared_mag_vect_complex_s16()`      |             |        |            |
| `bfp_squared_mag_vect_complex_s32()`      |             |        |            |
| `bfp_mag_vect_complex_s16()`              |             |        |            |
| `bfp_mag_vect_complex_s32()`              |             |        |            |
| `bfp_sum_complex_s16()`                   |             |        |            |
| `bfp_sum_complex_s32()`                   |             |        |            |
| `bfp_headroom_vect_ch_pair_s16()`         |             |        |            |
| `bfp_headroom_vect_ch_pair_s32()`         |             |        |            |
| @tab @tab               xs3_bfp_ch_pair.h                                  |||| 
| `bfp_shl_vect_ch_pair_s16()`              |             |        |            |
| `bfp_shl_vect_ch_pair_s32()`              |             |        |            |
| @tab @tab                   xs3_filters.h                                  |||| 
| @tab @tab            xs3_bfp_statistics.h                                  |||| 
| `bfp_abs_sum_s16()`                       |             |        |            |
| `bfp_abs_sum_s32()`                       |             |        |            |
| `bfp_mean_s16()`                          |             |        |            |
| `bfp_mean_s32()`                          |             |        |            |
| `bfp_energy_s16()`                        |             |        |            |
| `bfp_energy_s32()`                        |             |        |            |
| `bfp_rms_s16()`                           |             |        |            |
| `bfp_rms_s32()`                           |             |        |            |
| `bfp_max_s16()`                           |             |        |            |
| `bfp_max_s32()`                           |             |        |            |
| `bfp_min_s16()`                           |             |        |            |
| `bfp_min_s32()`                           |             |        |            |
| `bfp_argmax_s16()`                        |             |        |            |
| `bfp_argmax_s32()`                        |             |        |            |
| `bfp_argmin_s16()`                        |             |        |            |
| `bfp_argmin_s32()`                        |             |        |            |
| @tab @tab                       xs3_fft.h                                  |||| 
| `bfp_fft_forward_complex()`               |     X       |   X    |     X      |
| `bfp_fft_inverse_complex()`               |     X       |   X    |     X      |
| `bfp_fft_forward_mono()`                  |     X       |   X    |     X      |
| `bfp_fft_inverse_mono()`                  |     X       |   X    |     X      |
| `bfp_fft_forward_stereo()`                |     X       |   X    |     X      |
| `bfp_fft_inverse_stereo()`                |     X       |   X    |     X      |
| @tab @tab                       xs3_dct.h                                  |||| 
| `bfp_dct_forward()`                       |             |        |            |
| `bfp_dct_inverse()`                       |             |        |            |