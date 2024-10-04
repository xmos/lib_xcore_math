Example App: filter_demo
========================

The purpose of this application is to demonstrate, through example, how the functions of
``lib_xcore_math``'s filtering vector API may be used.

The filtering API currently supports three different filter types:

* 32-bit FIR Filter
* 16-bit FIR Filter
* 32-bit Biquad Filter

This example app presents simple demonstrations of how to use each of these filter types.

Building
********

After configuring the CMake project, the following commands can be used to build and run this
example application using the XK-EVK-XU316 board as a target:

::

    cmake -B build -G "Unix Makefiles"
    xmake -C build
    xrun --xscope filter_demo/bin/filter_demo.xe
