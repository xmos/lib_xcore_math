
API Function Status                 {#api_status}
===================

This table is unlikely to have actually captured everything that will be needed for an initial release (for example, it is currently unclear to me what low-level FFT functions are necessary). As new required API functions are identified they should be added to this table. Additionally, other functions (particularly utility functions) may have been written that haven't been included here.

### Low-Level Functions ###

| Function                                  | Implemented|| Tested | Documented |
| :---------------------------------------- | :--: | :--: | :----: | :--------: |
|                                           | ASM  |  C   |        |            | 
|  @tab @tab xs3_bfp_low.h                                                  ||||| 
| `xs3_vect_s16_set()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s32_set()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s16_headroom()`                 |  X   |  X   |   X    |     X      |
| `xs3_vect_s32_headroom()`                 |  X   |  X   |   X    |     X      |
| `xs3_vect_s16_shr()`                      |  X   |  X   |   X    |            |
| `xs3_vect_s32_shr()`                      |  X   |  X   |   X    |            |
| `xs3_vect_s16_shl()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s32_shl()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s16_add()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s32_add()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s16_sub()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s32_sub()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s16_mul()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s32_mul()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s16_scalar_mul()`               |  X   |  X   |   X    |     X      |
| `xs3_vect_s32_scalar_mul()`               |  X   |  X   |   X    |     X      |
| `xs3_vect_s16_abs()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s32_abs()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s16_sum()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s32_sum()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s16_dot()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s32_dot()`                      |  X   |  X   |   X    |     X      |
| `xs3_vect_s16_clip()`                     |  X   |  X   |   X    |     X      |
| `xs3_vect_s32_clip()`                     |  X   |  X   |   X    |     X      |
| `xs3_vect_s16_rect()`                     |  X   |  X   |   X    |     X      |
| `xs3_vect_s32_rect()`                     |  X   |  X   |   X    |     X      |
| `xs3_vect_s32_to_s16()`                   |  X   |  X   |   X    |     X      |
| `xs3_vect_s16_to_s32()`                   |  X   |  X   |   X    |     X      |
| `xs3_vect_s16_sqrt()`                     |  X   |  X   |   X    |            |
| `xs3_vect_s32_sqrt()`                     |  X   |  X   |   X    |            |
| `xs3_vect_s16_inverse()`                  |  X   |  X   |   X    |            |
| `xs3_vect_s32_inverse()`                  |  X   |  X   |   X    |            |
|  @tab @tab xs3_bfp_ch_pair_low.h                                          ||||| 
| `xs3_vect_ch_pair_s16_set()`              |  X   |  X   |   X    |            |
| `xs3_vect_ch_pair_s32_set()`              |  X   |  X   |   X    |            |
| `xs3_vect_ch_pair_s16_headroom()`         |  X   |  X   |   X    |            |
| `xs3_vect_ch_pair_s32_headroom()`         |  X   |  X   |   X    |            |
| `xs3_vect_ch_pair_s16_shr()`              |  X   |  X   |   X    |            |
| `xs3_vect_ch_pair_s32_shr()`              |  X   |  X   |   X    |            |
| `xs3_vect_ch_pair_s16_shl()`              |  X   |  X   |   X    |            |
| `xs3_vect_ch_pair_s32_shl()`              |  X   |  X   |   X    |            |
|  @tab @tab xs3_bfp_complex_low.h                                          ||||| 
| `xs3_vect_complex_s16_set()`              |  X   |  X   |   X    |            |
| `xs3_vect_complex_s32_set()`              |  X   |  X   |   X    |     X      |
| `xs3_vect_complex_s16_headroom()`         |  X   |  X   |   X    |            |
| `xs3_vect_complex_s32_headroom()`         |  X   |  X   |   X    |            |
| `xs3_vect_complex_s16_shr()`              |  X   |  X   |   X    |            |
| `xs3_vect_complex_s32_shr()`              |  X   |  X   |   X    |            |
| `xs3_vect_complex_s16_shl()`              |  X   |  X   |   X    |            |
| `xs3_vect_complex_s32_shl()`              |  X   |  X   |   X    |            |
| `xs3_vect_complex_s16_add()`              |  X   |  X   |   X    |            |
| `xs3_vect_complex_s32_add()`              |  X   |  X   |   X    |            |
| `xs3_vect_complex_s16_sub()`              |  X   |  X   |   X    |            |
| `xs3_vect_complex_s32_sub()`              |  X   |  X   |   X    |            |
| `xs3_vect_complex_s16_mul()`              |  X   |  X   |   X    |     X      |
| `xs3_vect_complex_s32_mul()`              |  X   |  X   |   X    |     X      |
| `xs3_vect_complex_s16_complex_mul()`      |  X   |  X   |   X    |     X      |
| `xs3_vect_complex_s32_complex_mul()`      |  X   |  X   |   X    |     X      |
| `xs3_vect_complex_s16_complex_conj_mul()` |  X   |  X   |   X    |     X      |
| `xs3_vect_complex_s32_complex_conj_mul()` |  X   |  X   |   X    |     X      |
| `xs3_vect_complex_s16_complex_scal_mul()` |  X   |  X   |   X    |     X      |
| `xs3_vect_complex_s32_complex_scal_mul()` |  X   |  X   |   X    |     X      |
| `xs3_vect_complex_s16_scalar_mul()`       |  X   |  X   |   X    |            |
| `xs3_vect_complex_s32_scalar_mul()`       |  X   |  X   |   X    |            |
| `xs3_complex_s16_to_complex_s32()`        |  X   |  X   |   X    |     X      |
| `xs3_complex_s32_to_complex_s16()`        |  X   |  X   |   X    |     X      |
| `xs3_vect_complex_s16_squared_mag()`      |  X   |  X   |   X    |     X      |
| `xs3_vect_complex_s32_squared_mag()`      |  X   |  X   |   X    |     X      |
| `xs3_vect_complex_s16_mag()`              |  X   |  X   |   X    |            |
| `xs3_vect_complex_s32_mag()`              |  X   |  X   |   X    |            |
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
| `xs3_filter_fir_s32()`                    |  X   |  X   |   X    |            | 
| `xs3_filter_fir_s16()`                    |  X   |  X   |   X    |            |
| `xs3_filter_biquad_s32()`                 |  X   |  X   |   X    |            |
| `xs3_filter_biquads_s32()`                |  X   |  X   |   X    |            |
|  @tab @tab xs3_statistics_low.h                                           ||||| 
| `xs3_vect_s16_abs_sum()`                  |  X   |  X   |   X    |            |
| `xs3_vect_s32_abs_sum()`                  |  X   |  X   |   X    |            |
| `xs3_vect_s16_energy()`                   |  X   |  X   |   X    |            |
| `xs3_vect_s32_energy()`                   |  X   |  X   |   X    |            |
| `xs3_vect_s16_max()`                      |  X   |  X   |   X    |            |
| `xs3_vect_s32_max()`                      |  X   |  X   |   X    |            |
| `xs3_vect_s16_min()`                      |  X   |  X   |   X    |            |
| `xs3_vect_s32_min()`                      |  X   |  X   |   X    |            |
| `xs3_vect_s16_argmax()`                   |  X   |  X   |   X    |            |
| `xs3_vect_s32_argmax()`                   |  X   |  X   |   X    |            |
| `xs3_vect_s16_argmin()`                   |  X   |  X   |   X    |            |
| `xs3_vect_s32_argmin()`                   |  X   |  X   |   X    |            |


### High-Level API Functions ###

The high-level API functions are written entirely in C (perhaps C++ wrappers will eventually be made available), to implement block floating-point arithmetic which manage headroom and exponents for the user. They use the low-level API to accomplish their work.

| Function                                  | Implemented | Tested | Documented |
| :---------------------------------------- | :---------: | :----: | :--------: |
| @tab @tab                  xs3_bfp_init.h                                  |||| 
| `bfp_vect_s16_init()`                     |      X      |   X    |     X      |
| `bfp_vect_s32_init()`                     |      X      |   X    |     X      |
| `bfp_vect_complex_s16_init()`             |      X      |   X    |     X      |
| `bfp_vect_complex_s32_init()`             |      X      |   X    |     X      |
| `bfp_vect_ch_pair_s16_init()`             |      X      |   X    |     X      |
| `bfp_vect_ch_pair_s32_init()`             |      X      |   X    |     X      |
| `bfp_vect_s16_set()`                      |      X      |   X    |     X      |
| `bfp_vect_s32_set()`                      |      X      |   X    |     X      |
| `bfp_vect_complex_s16_set()`              |      X      |   X    |     X      |
| `bfp_vect_complex_s32_set()`              |      X      |   X    |     X      |
| `bfp_vect_ch_pair_s16_set()`              |      X      |   X    |     X      |
| `bfp_vect_ch_pair_s32_set()`              |      X      |   X    |     X      |
| @tab @tab                       xs3_bfp.h                                  |||| 
| `bfp_vect_s16_headroom()`                 |      X      |   X    |     X      |
| `bfp_vect_s32_headroom()`                 |      X      |   X    |     X      |
| `bfp_vect_s16_shl()`                      |      X      |   X    |     X      |
| `bfp_vect_s32_shl()`                      |      X      |   X    |     X      |
| `bfp_vect_s16_add()`                      |      X      |   X    |     X      |
| `bfp_vect_s32_add()`                      |      X      |   X    |     X      |
| `bfp_vect_s16_sub()`                      |      X      |   X    |     X      |
| `bfp_vect_s32_sub()`                      |      X      |   X    |     X      |
| `bfp_vect_s16_mul()`                      |      X      |   X    |     X      |
| `bfp_vect_s32_mul()`                      |      X      |   X    |     X      |
| `bfp_vect_s16_scalar_mul()`               |      X      |   X    |     X      |
| `bfp_vect_s32_scalar_mul()`               |      X      |   X    |     X      |
| `bfp_vect_s16_abs()`                      |      X      |   X    |     X      |
| `bfp_vect_s32_abs()`                      |      X      |   X    |     X      |
| `bfp_vect_s16_sum()`                      |      X      |   X    |     X      |
| `bfp_vect_s32_sum()`                      |      X      |   X    |     X      |
| `bfp_vect_dot_s16()`                      |      X      |   X    |            |
| `bfp_vect_dot_s32()`                      |      X      |   X    |            |
| `bfp_vect_s16_clip()`                     |      X      |   X    |            |
| `bfp_vect_s32_clip()`                     |      X      |   X    |            |
| `bfp_vect_s16_rect()`                     |      X      |   X    |     X      |
| `bfp_vect_s32_rect()`                     |      X      |   X    |     X      |
| `bfp_s32_to_s16()`                        |      X      |   X    |     X      |
| `bfp_s16_to_s32()`                        |      X      |   X    |     X      |
| `bfp_vect_s16_sqrt()`                     |      X      |   X    |            |
| `bfp_vect_s32_sqrt()`                     |      X      |   X    |            |
| `bfp_vect_s16_inverse()`                  |      X      |   X    |            |
| `bfp_vect_s32_inverse()`                  |      X      |   X    |            |
| @tab @tab               xs3_bfp_complex.h                                  |||| 
| `bfp_vect_complex_s16_headroom()`         |      X      |   X    |            |
| `bfp_vect_complex_s32_headroom()`         |      X      |   X    |            |
| `bfp_vect_complex_s16_shl()`              |      X      |   X    |            |
| `bfp_vect_complex_s32_shl()`              |      X      |   X    |            |
| `bfp_vect_complex_s16_mul()`              |      X      |   X    |            |
| `bfp_vect_complex_s32_mul()`              |      X      |   X    |            |
| `bfp_vect_complex_s16_complex_mul()`      |      X      |   X    |            |
| `bfp_vect_complex_s32_complex_mul()`      |      X      |   X    |            |
| `bfp_vect_complex_s16_complex_conj_mul()` |      X      |   X    |            |
| `bfp_vect_complex_s32_complex_conj_mul()` |      X      |   X    |            |
| `bfp_vect_complex_s16_scalar_mul()`       |      X      |   X    |            |
| `bfp_vect_complex_s32_scalar_mul()`       |      X      |   X    |            |
| `bfp_vect_complex_s16_complex_scal_mul()` |      X      |   X    |            |
| `bfp_vect_complex_s32_complex_scal_mul()` |      X      |   X    |            |
| `bfp_vect_complex_s16_add()`              |      X      |   X    |            |
| `bfp_vect_complex_s32_add()`              |      X      |   X    |            |
| `bfp_vect_complex_s16_sub()`              |      X      |   X    |            |
| `bfp_vect_complex_s32_sub()`              |      X      |   X    |            |
| `bfp_vect_complex_s16_to_complex_s32()`   |      X      |   X    |            |
| `bfp_vect_complex_s32_to_complex_s16()`   |      X      |   X    |            |
| `bfp_vect_complex_s16_squared_mag()`      |      X      |   X    |            |
| `bfp_vect_complex_s32_squared_mag()`      |      X      |   X    |            |
| `bfp_vect_complex_s16_mag()`              |      X      |   X    |            |
| `bfp_vect_complex_s32_mag()`              |      X      |   X    |            |
| `bfp_vect_complex_s16_sum()`              |      X      |   X    |            |
| `bfp_vect_complex_s32_sum()`              |      X      |   X    |            |
| @tab @tab               xs3_bfp_ch_pair.h                                  |||| 
| `bfp_vect_ch_pair_s16_headroom()`         |      X      |   X    |            |
| `bfp_vect_ch_pair_s32_headroom()`         |      X      |   X    |            |
| `bfp_vect_ch_pair_s16_shl()`              |      X      |   X    |            |
| `bfp_vect_ch_pair_s32_shl()`              |      X      |   X    |            |
| @tab @tab                   xs3_filters.h                                  |||| 
| @tab @tab            xs3_bfp_statistics.h                                  |||| 
| `bfp_vect_s16_abs_sum()`                  |      X      |   X    |            |
| `bfp_vect_s32_abs_sum()`                  |      X      |   X    |            |
| `bfp_vect_s16_mean()`                     |      X      |   X    |            |
| `bfp_vect_s32_mean()`                     |      X      |   X    |            |
| `bfp_vect_s16_energy()`                   |      X      |   X    |            |
| `bfp_vect_s32_energy()`                   |      X      |   X    |            |
| `bfp_vect_s16_rms()`                      |      X      |   X    |            |
| `bfp_vect_s32_rms()`                      |      X      |   X    |            |
| `bfp_vect_s16_max()`                      |      X      |   X    |            |
| `bfp_vect_s32_max()`                      |      X      |   X    |            |
| `bfp_vect_s16_min()`                      |      X      |   X    |            |
| `bfp_vect_s32_min()`                      |      X      |   X    |            |
| `bfp_vect_s16_argmax()`                   |      X      |   X    |            |
| `bfp_vect_s32_argmax()`                   |      X      |   X    |            |
| `bfp_vect_s16_argmin()`                   |      X      |   X    |            |
| `bfp_vect_s32_argmin()`                   |      X      |   X    |            |
| @tab @tab                       xs3_fft.h                                  |||| 
| `bfp_fft_forward_complex()`               |      X      |   X    |     X      |
| `bfp_fft_inverse_complex()`               |      X      |   X    |     X      |
| `bfp_fft_forward_mono()`                  |      X      |   X    |     X      |
| `bfp_fft_inverse_mono()`                  |      X      |   X    |     X      |
| `bfp_fft_forward_stereo()`                |      X      |   X    |     X      |
| `bfp_fft_inverse_stereo()`                |      X      |   X    |     X      |
| @tab @tab                       xs3_dct.h                                  |||| 
| `bfp_dct_forward()`                       |             |        |            |
| `bfp_dct_inverse()`                       |             |        |            |