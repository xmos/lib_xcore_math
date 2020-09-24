

Introduction                   {#mainpage}
============

\tableofcontents

`lib_xs3_math` is a library of optimized math functions for taking advantage of the vector processing unit (VPU) on the XMOS XS3 architecture.

See @ref getting_started to get going.

### API ### 

To use the functions in this library in an application, include `xs3_math.h`.

The API is documented in the following header files.

#### High Level ####

The high-level API is a block floating-point (BFP) API which manages the vector headroom and exponents for you. We recommend
using the high-level API where possible.

Functions in the high-level API generally begin with the prefix `bfp_`.

xs3_bfp_init.h: API for initializing BFP vectors.

xs3_bfp.h: BFP API for real arithmetic functions.

xs3_bfp_complex.h: BFP API for complex arithmetic functions.

xs3_fft.h: BFP API for FFTs.

#### Low Level ####

The low-level API is more powerful, but requires manual management of vector headroom and exponents. If not managed 
correctly, excessive information may be lost due to saturation or underflow. 

Functions in the low-level API generally begin with the prefix `xs3_`.

xs3_bfp_init_low.h: Low level vector initialization functions.

xs3_bfp_low.h: Low level arithmetic functions.