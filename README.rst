lib_xcore_math
==============

Library Info
############

Summary
-------

``lib_xcore_math`` is an arithmetic and DSP library which leverages the XS3 Vector Processing Unit
(VPU) to accelerate costly operations on vectors of 16- or 32-bit data.  Included are functions for
block floating-point arithmetic, fast Fourier transforms, discrete cosine transforms, linear
filtering and more.

Repository Structure
--------------------

* `/lib_xcore_math/ <https://github.com/xmos/lib_xcore_math/lib_xcore_math>`_ - The ``lib_xcore_math`` library directory.

  * `api/ <https://github.com/xmos/lib_xcore_math/tree/develop/lib_xcore_math/api/>`_ - Headers containing the public API for ``lib_xcore_math``.
  * `script/ <https://github.com/xmos/lib_xcore_math/tree/develop/lib_xcore_math/script/>`_ - Scripts used for source generation.
  * `src/ <https://github.com/xmos/lib_xcore_math/tree/develop/lib_xcore_math/src/>`_ - Library source code.

* `/doc/ <https://github.com/xmos/lib_xcore_math/tree/develop/lib_xcore_math/doc/>`_ - Sphinx library documentation source and build directory.
* `/example/ <https://github.com/xmos/lib_xcore_math/tree /develop/example/>`_ - Example applications for ``lib_xcore_math``.
* `/test/ <https://github.com/xmos/lib_xcore_math/tree/develop/test/>`_ - Unit test projects for ``lib_xcore_math``.


Requirements
------------

* xTIMEcomposer tools version 15.0.6 or later
* CMake (for building the library and unit tests, version 3.14 or later)
* Doxygen (for building documentation)
* Python 3 (for running source generation scripts, if necessary)

The recommended make tools depend on the development platform:

(Linux and MacOS): GNU Make - Available through your system package manager.

Ninja (Windows): Ninja - Follow install instructions at https://ninja-build.org/ or on Windows
install with ``winget`` by running the following commands in PowerShell:

.. code-block:: PowerShell

		# Install
		winget install Ninja-build.ninja
		# Reload user Path
		$env:Path=[System.Environment]::GetEnvironmentVariable("Path","User")

API Structure
-------------

This library is organized around several sub-APIs.  These APIs collect the provided operations into
coherent groups based on the kind of operation or the types of object being acted upon.

The current APIs are:

* Block Floating-Point Vector API
* Vector/Array API
* Scalar API
* Linear Filtering API
* Fast Fourier Transform API
* Discrete Cosine Transform API

See the full documentation for more details about each of these as well as usage examples.

Getting and Building
####################

Clone the Repository
--------------------

To clone this repository you may use the following: ::

    git clone --recurse-submodules git@github.com:xmos/lib_xcore_math

.. note::

   This repository uses submodules to include the XMOS CMake toolchain

Including lib_xcore_math in External Applications
-------------------------------------------------

``lib_xcore_math`` can be compiled for both x86 platforms and xs3a processors.

On x86 platforms you can develop DSP algorithms and test them for functional correctness;
this is an optional step before porting the library to an xs3a device.

.. note::

   The instructions in this section refer to the compilation for xs3a; the steps to compile for the x86 platforms are the same,
   except that the XTC build tools are not necessary and the default toolchain must be used. This can be done by skipping the option
   ``--toolchain=`` pointing to ``xs3a.cmake`` when configuring CMake.

``lib_xcore_math`` may be included in your own applications either as source to be compiled by your
application or as a static library to be linked into your own application. This library uses CMake
to manage build configurations.

On Linux and MacOS, to configure your CMake build environment for ``lib_xcore_math`` to compile for xs3a,
from the root of the cloned repository, the following command may be used (ensure that the XTC build
tools are on your path): ::

    mkdir build && cd build && cmake --toolchain=../etc/xmos_cmake_toolchain/xs3a.cmake ..

Then to actually build the library as a static binary just use the ``make`` command from the
``build`` directory.

The unit tests and example applications are built by default when running the commands above.

On Windows, to configure your CMake build environment for ``lib_xcore_math``,
from the root of the cloned repository, the following command may be used (ensure that the XTC build
tools are on your path): ::

    mkdir build && cd build && cmake --toolchain=../etc/xmos_cmake_toolchain/xs3a.cmake -G Ninja ..

Then to actually build the library as a static binary just use the ``ninja`` command from the
``build`` directory.

The unit tests and example applications are built by default when running the commands above.

If you wish to include ``lib_xcore_math`` in your own application as a static library, the generated
``lib_xcore_math.a`` can then be linked into your own application. Be sure to also add
``lib_xcore_math/api`` as an include directory for your project.

To incorporate ``lib_xcore_math`` into your own CMake project, you have two options. You can either
add ``/lib_xcore_math`` as a CMake subdirectory (via ``add_subdirectory()``), which will include it
as a static library. Or, to include it as a source library you can include
``/lib_xcore_math/lib_xcore_math.cmake`` in your application's CMake project, which will populate
various CMake variables (prepended with ``LIB_XCORE_MATH_``) with the source files, include
directories and build flags required. See ``/lib_xcore_math/lib_xcore_math.cmake`` for the specific
variables.

For other build systems

* Add ``lib_xcore_math/api`` as an include directory
* Add all .c files within ``lib_xcore_math/src/`` *except* for those within ``lib_xcore_math/src/arch/ref``
* Add all .S files within ``lib_xcore_math/src/arch/xs3`` as source files

  * These are assembly files and should be compiled with ``xcc`` as are the C files.

Then, from your source files, include ``xmath/xmath.h``.

Unit Tests and Examples
-----------------------

This project uses CMake to build the unit test and example applications. Use the steps described above to
configure and build the unit test and example applications. Both unit test and example projects currently target the
xcore.ai explorer board and x86 platforms. All unit tests are currently in the `/test/
<https://github.com/xmos/lib_xcore_math/tree/develop/test/>`_ directory:

* `/test/ <https://github.com/xmos/lib_xcore_math/tree/develop/test/>`_ - Unit test projects for ``lib_xcore_math``:

  * `bfp_tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/test/bfp_tests/>`_ - BFP unit tests
  * `dct_tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/test/dct_tests/>`_ - DCT unit tests
  * `filter_tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/test/filter_tests/>`_ - Filtering unit tests
  * `fft_tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/test/fft_tests/>`_ - FFT unit tests
  * `scalar_tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/test/scalar_tests/>`_ - Scalar op unit tests
  * `vect_tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/test/vect_tests/>`_ - Vector op unit tests
  * `xs3_tests/ <https://github.com/xmos/lib_xcore_math/tree/develop/test/xs3_tests/>`_ - XS3-specific unit tests

All examples are currently in the `/example/
<https://github.com/xmos/lib_xcore_math/tree/develop/example/>`_ directory:

* `/example/ <https://github.com/xmos/lib_xcore_math/tree/develop/example/>`_ - Example projects for ``lib_xcore_math``:

  * `bfp_demo/ <https://github.com/xmos/lib_xcore_math/tree/develop/example/bfp_demo/>`_ - BFP demo
  * `filter_demo/ <https://github.com/xmos/lib_xcore_math/tree/develop/example/filter_demo/>`_ - Filtering demo
  * `fft_demo/ <https://github.com/xmos/lib_xcore_math/tree/develop/example/fft_demo/>`_ - FFT demo
  * `vect_demo/ <https://github.com/xmos/lib_xcore_math/tree/develop/example/vect_demo/>`_ - Vector op demo

Each example above has a ReadMe file with a short description.


All unit tests and examples are built and executed in a similar manner. The following shows how to do this with
the BFP unit tests.

BFP Unit Tests
**************

This application runs unit tests for the various 16- and 32-bit BFP vectorized arithmetic functions.
This application is located at `/test/bfp_tests/
<https://github.com/xmos/lib_xcore_math/tree/develop/test/bfp_tests>`_.

To execute the BFP unit tests on the explorer board, from your CMake build directory use the
following (after ensuring that the hardware is connected and drivers properly installed): ::

    xrun --xscope test/bfp_tests/bfp_tests.xe

Or, to run the unit tests in the software simulator: ::

    xsim test/bfp_tests/bfp_tests.xe

Note that running the unit tests in the simulator may be *very* slow.

To execute the BFP unit tests built for an x86 host platform, from your CMake build directory run on Linux and MacOS: ::

   ./test/bfp_tests/bfp_tests -v

and on Windows: ::

   test\bfp_tests\bfp_tests.exe -v

where ``-v`` is an optional argument to increase verbosity.

Building Documentation
----------------------

The following information is intended for XMOS users. Pre-built documentation may be found on the
`XMOS website <https://www.xmos.com>`_.

This project currently uses Doxygen and Sphinx for library and API documentation. API functions
include embedded documentation with their declarations in their corresponding header files, however,
MathJax is used to render equations, and as such, building the stand-alone HTML documentation is
highly recommended. In addition, several non-embedded notes and guides are included in the
stand-alone documentation (these can also be found at `/doc/src/
<https://github.com/xmos/lib_xcore_math/tree/develop/doc/>`_ ).

 To build the stand-alone documentation as HTML a Doxygen install will be required. The
 documentation has been written against Doxygen version 1.8; your mileage may vary with other
 versions.

With Doxygen on your path, the documentation can be built by calling ``doxygen`` from within the
`/lib_xcore_math/doc/ <https://github.com/xmos/lib_xcore_math/tree/develop/lib_xcore_math/doc/>`_
directory.

The documentation will be generated within the ``/doc/build/`` directory. To view the
HTML version of the documentation, open ``/doc/build/html/index.html`` in a browser.

