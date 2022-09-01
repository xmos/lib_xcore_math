Example App: vect_demo
======================

The purpose of this application is to demonstrate, through example, how the arithmetic functions of
``lib_xcore_math``'s vector API may be used.

In general the vector arithmetic API comprises the functions in this library whose names begin with
``vect_*``, such as ``vect_s32_mul()`` for element-wise multiplication of 32-bit vectors, and
``vect_complex_s16_scale()`` for multiplying a complex 16-bit vector by a complex scalar.

We assume that where the low-level API is being used it is because some behavior other than the
default behavior of the high-level block floating-point API is required. Given that, rather than
showcasing the breadth of operations available, this example examines first how to achieve
comparable behavior to the BFP API, and then ways in which that behavior can be modified.

Building
********

After configuring the CMake project, the following commands can be used to build and run this
example application using the XCORE-AI-EXPLORER board as a target:

::

    make vect_demo
    xrun --xscope example/vect_demo/vect_demo.xe
