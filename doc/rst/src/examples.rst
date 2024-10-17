.. _examples:

********************
Example Applications
********************

Several example applications are offered to demonstrate use of the ``lib_xcore_math`` APIs through
simple code examples.

  * ``bfp_demo`` - Demonstration of the block floating-point arithmetic API
  * ``vect_demo`` - Demonstration of the low-level vectorized arithmetic API
  * ``fft_demo`` - Demonstration of the Fast Fourier Transform API
  * ``filter_demo`` - Demonstration of the filtering API

This section assumes you have downloaded and installed the `XMOS XTC tools <https://www.xmos.com/software-tools/>`_
(see `README` for required version).
Installation instructions can be found `here <https://xmos.com/xtc-install-guide>`_.

Particular attention should be paid to the section `Installation of required third-party tools
<https://www.xmos.com/documentation/XM-014363-PC-10/html/installation/install-configure/install-tools/install_prerequisites.html>`_.

The application examples uses the `xcommon-cmake <https://www.xmos.com/file/xcommon-cmake-documentation/?version=latest>`_
build system as bundled with the XTC tools.

Building Examples
=================

To build the applications, from an XTC command prompt run the following commands in the
`lib_xcore_math/examples` directory::

    cmake -B build -G "Unix Makefiles"
    xmake -C build

Individual examples can be built using a command similar to the following::

    xmake -C build EXAMPLE_NAME

where ``EXAMPLE_NAME`` is the example to build.

Running Examples
================

Once built, the example ``EXAMPLE_NAME`` can be run on the `XK-EVK-XU316` board using the following
command::

    xrun --xscope examples/EXAMPLE_NAME/bin/EXAMPLE_NAME.xe

For instance, to run the ``bfp_demo`` example, use::

    xrun --xscope examples/bfp_demo/bin/bfp_demo.xe

To run the example using the ``xcore`` simulator instead, use::

    xsim examples/EXAMPLE_NAME/bin/EXAMPLE_NAME.xe

bfp_demo
========

The purpose of this example application is to demonstrate how the arithmetic functions of
``lib_xcore_math``'s block floating-point API may be used.

In it, three 32-bit BFP vectors are allocated, initialized and filled with random data. Then several
BFP operations are applied using those vectors as inputs and/or outputs.

The example only demonstrates the real 32-bit arithmetic BFP functions (that is, functions with
names ``bfp_s32_*``). The real 16-bit (``bfp_s16_*``), complex 32-bit (``bfp_complex_s32_*``) and
complex 16-bit (``bfp_complex_s16_*``) functions all use similar naming conventions.

vect_demo
=========

The purpose of this example application is to demonstrate how the arithmetic functions of
``lib_xcore_math``'s lower-level vector API may be used.

In general the low-level arithmetic API are the functions in this library whose names begin with
``vect_*``, such as :c:func:`vect_s32_mul()` for element-wise multiplication of 32-bit vectors, and
:c:func:`vect_complex_s16_scale()` for multiplying a complex 16-bit vector by a complex scalar.

We assume that where the low-level API is being used it is because some behavior other than the
default behavior of the high-level block floating-point API is required. Given that, rather than
showcasing the breadth of operations available, this example examines first how to achieve
comparable behavior to the BFP API, and then ways in which that behavior can be modified.

fft_demo
========

The purpose of this example application is to demonstrate how the FFT functions of
``lib_xcore_math``'s block floating-point API may be used.

In this example we demonstrate each of the offered forward and inverse FFTs of the BFP API.

filter_demo
===========

The purpose of this example application is to demonstrate how the functions of
``lib_xcore_math``'s filtering vector API may be used.

The filtering API currently supports three different filter types:

  * 32-bit FIR Filter
  * 16-bit FIR Filter
  * 32-bit Biquad Filter

This example application presents simple demonstrations of how to use each of these filter types.

