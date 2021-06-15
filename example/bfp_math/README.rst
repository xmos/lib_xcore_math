Example App: bfp_math
=====================

The purpose of this application is to demonstrate, through example, how the arithmetic functions of ``lib_xs3_math``'s block floating-point API may be used.

In it, three 32-bit BFP vectors are allocated, initialized and filled with random data. Then several BFP operations are
applied using those vectors as inputs and/or outputs.

The example only demonstrates the real 32-bit arithmetic BFP functions (that is, functions with names ``bfp_s32_*``). The real 16-bit (``bfp_s16_*``), complex 32-bit (``bfp_complex_s32_*``) and complex 16-bit (``bfp_complex_s16_*``) functions all use very similar conventions as the real 32-bit functions.

Building
********

After configuring the CMake project, the following commands can be used to build and run this example application using the XCORE-AI-EXPLORER board as a target:

::

    make bfp_math
    xrun --xscope example/bfp_math/bfp_math.xe
