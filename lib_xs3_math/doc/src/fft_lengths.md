
Changing Library FFT Length Support             {#fft_length_support}
===================================

When computing DFTs this library relies on one or both of a pair of look-up tables which contain portions of the 
Discrete Fourier Transform matrix. The table(s) can be found in `lib_xs3_math/src/vect/xs3_fft_lut.c`, with 
accompanying definitions and macros in `lib_xs3_math/src/vect/xs3_fft_lut.h`.

By default the tables contain the coefficients necessary to perform forward or inverse DFTs of up to 1024 points. If 
larger DFTs are required, or if the maximum required DFT size is known to be less than 1024 points, these look-up tables 
can be regenerated using a python script provided with this library. The script is located at 
`lib_xs3_math/scripts/gen_fft_table.py`.

The two look-up tables correspond to the decimation-in-time and decimation-in-frequency FFT algorithms, and the run-time
symbols for the tables are `xs3_dit_fft_lut` and `xs3_dif_fft_lut` respectively. Each table contains @math{N-4} complex
32-bit values, with a size of @math{8\cdot (N-4)} bytes each.

To change the maximum FFT length to @math{16384} (@math{=2^{14}}), supporting only the decimation-in-time algorithm, the 
following command can be used:

@code
    python lib_xs3_math/script/gen_fft_table.py --dit --max_fft_log2 14
@endcode

and the generated files can replace the header and source files specified above.

Use the `--help` flag with `gen_fft_table.py` for a more detailed description of its syntax and parameters.