Example App: fft_demo
=====================

The purpose of this application is to demonstrate, through example, how the FFT functions of
``lib_xcore_math``'s FFT API may be used.

In this example we demonstrate several of the offered forward and inverse BFP FFTs.

Building
********

After configuring the CMake project, the following commands can be used (from the build directory)
to build and run this example application using the XK-EVK-XU316 board as a target:

::

    cmake -B build -G "Unix Makefiles"
    xmake -C build
    xrun --xscope fft_demo/bin/fft_demo.xe
