.. _examples:

####################
Example Applications
####################

Several example applications are offered to demonstrate use of the ``lib_xs3_math`` APIs through
actual, simple code examples.



Building Examples
=================

After configuring the CMake project (with the ``BUILD_EXAMPLES`` enabled), all the examples can
be built by using the ``make`` command within the build directory.  Individual examples can be built
using ``make EXAMPLE_NAME``, where ``EXAMPLE_NAME`` is the example to build. 



Running Examples
================

To run example ``EXAMPLE_NAME`` on the XCORE-AI-EXPLORER board, from the CMake build directory, run
the following command after building:

::

    xrun --xscope example/EXAMPLE_NAME/EXAMPLE_NAME.xe


To run the example using the xCore simulator instead, use:

::

    xsim example/EXAMPLE_NAME/EXAMPLE_NAME.xe



bfp_demo
========

The purpose of this application is to demonstrate, through example, how the arithmetic functions of
``lib_xs3_math``'s block floating-point API may be used.

In it, three 32-bit BFP vectors are allocated, initialized and filled with random data. Then several 
BFP operations are applied using those vectors as inputs and/or outputs.

The example only demonstrates the real 32-bit arithmetic BFP functions (that is, functions with names 
``bfp_s32_*``). The real 16-bit (``bfp_s16_*``), complex 32-bit (``bfp_complex_s32_*``) and complex 
16-bit (``bfp_complex_s16_*``) functions all use very similar conventions as the real 32-bit functions.



vect_demo
=========

The purpose of this application is to demonstrate, through example, how the arithmetic functions of 
``lib_xs3_math``'s low-level vector API may be used.

In general the low-level arithmetic API are the functions in this library whose names begin with 
``xs3_vect_*``, such as :c:func:`xs3_vect_s32_mul()` for element-wise multiplication of 32-bit 
vectors, and :c:func:`xs3_vect_complex_s16_scale()` for multiplying a complex 16-bit vector by a 
complex scalar.

We assume that where the low-level API is being used it is because some behavior other than the 
default behavior of the high-level block floating-point API is required. Given that, rather than 
showcasing the breadth of operations available, this example examines first how to achieve comparable 
behavior to the BFP API, and then ways in which that behavior can be modified.



fft_demo
========

The purpose of this application is to demonstrate, through example, how the FFT functions of 
``lib_xs3_math``'s block floating-point API may be used.

In this example we demonstrate each of the offered forward and inverse FFTs of the BFP API.



filter_demo
===========

The purpose of this application is to demonstrate, through example, how the functions of 
``lib_xs3_math``'s filtering vector API may be used.

The filtering API currently supports three different filter types:

* 32-bit FIR Filter
* 16-bit FIR Filter
* 32-bit Biquad Filter

This example app presents simple demonstrations of how to use each of these filter types.

