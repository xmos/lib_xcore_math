.. lib_xs3_math documentation master file, created by
   sphinx-quickstart on Mon Jul 12 11:09:43 2021.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Introduction
============

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   src/getting_started
   src/notes
   src/fft_lengths


``lib_xs3_math`` is a library of optimized math functions for taking advantage of the vector processing unit (VPU) on the XMOS XS3 architecture.

See @ref getting_started to get going.

On GitHub
---------

Get the latest version of ``lib_xs3_math`` at https://github.com/xmos/lib_xs3_math.

API
---

To use the functions in this library in an application, include ``bfp_math.h`` for the high-level block floating-point 
functions, or ``xs3_math.h`` for only the low-level functions.

The API is documented in the following API header files.


======================  =======================================================
File                    Contains 
======================  =======================================================
xs3_math_types.h        Types defined in and used by this library
bfp/bfp_init.h          BFP vector initialization functions
bfp/bfp_fft.h           Block floating-point FFT functions
bfp/bfp.h               16- and 32-bit arithmetic function for BFP vectors
bfp/bfp_complex.h       Operations on complex block floating-point vectors
bfp/bfp_ch_pair.h       Operations on block floating-point channel-pair vectors
vect/xs3_fft.h          Low-level FFT functions
vect/xs3_filters.h      Filtering (FIR/Biquad) functions
vect/xs3_vect_s32.h     32-bit low-level arithmetic functions
vect/xs3_vect_s16.h     16-bit low-level arithmetic functions
xs3_math_conf.h         Compile-time configuration options
xs3_util.h              Various useful macros and scalar functions
xs3_vpu_info.h          Various macros and enums 
======================  =======================================================


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
