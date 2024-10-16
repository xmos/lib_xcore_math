
**********
Unit tests
**********

This project uses `XCommon CMake` to build the unit tests in a similar fashion to the examples.

Unit tests target the `XK-EVK-XU316` board and x86 platforms.
All unit tests are located in the */tests/* directory:

* `/tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/tests/>`_ - Unit test projects for ``lib_xcore_math``:

  * `bfp_tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/tests/bfp_tests/>`_ - BFP unit tests
  * `dct_tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/tests/dct_tests/>`_ - DCT unit tests
  * `filter_tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/tests/filter_tests/>`_ - Filtering unit tests
  * `fft_tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/tests/fft_tests/>`_ - FFT unit tests
  * `scalar_tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/tests/scalar_tests/>`_ - Scalar op unit tests
  * `vect_tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/tests/vect_tests/>`_ - Vector op unit tests
  * `xs3_tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/tests/xs3_tests/>`_ - XS3-specific unit tests

All unit tests and examples are built and executed in a similar manner. The following shows how to do this with
the BFP unit tests.

BFP unit tests
==============

This application runs unit tests for the various 16- and 32-bit BFP vectorized arithmetic functions.
This application is located at `/tests/bfp_tests/
<https://github.com/xmos/lib_xcore_math/tree/develop/tests/bfp_tests>`_.

To build the test, from an XTC command prompt run the following commands in the
`lib_xcore_math/tests/bfp_tests` directory::

    cmake -B build -G "Unix Makefiles"
    xmake -C build

To execute the BFP unit tests on the `XK-EVK-XU316`, use the
following (after ensuring that the hardware is connected and drivers properly installed): ::

    xrun --xscope bin/bfp_tests.xe

Or, to run the unit tests in the software simulator: ::

    xsim bin/bfp_tests.xe

.. warning::

    Running the unit tests in the simulator may be *very* slow.

To execute the BFP unit tests built for an x86 host platform, configure the build using the
``NATIVE_BUILD`` option: ::

    cmake -B build_x86 -G "Unix Makefiles" -D BUILD_NATIVE=TRUE
    xmake -C build_x86

on Linux and macOS run the tests as follows: ::

    bin/bfp_tests/bfp_tests -v

and on Windows: ::

   bin\bfp_tests\bfp_tests.exe -v

where ``-v`` is an optional argument to increase verbosity.

