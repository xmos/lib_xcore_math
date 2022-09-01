lib_xs3_math
============

Library Info
############

Summary
-------

``lib_xs3_math`` is an arithmetic and DSP library which leverages the XS3 Vector Processing Unit (VPU) to accelerate costly operations on vectors of 16- or 32-bit data.

This library is a work in progress and is likely to change significantly in the near future.

Repository Structure
--------------------

* `lib_xs3_math/ <https://github.com/xmos/lib_xs3_math>`_ - The actual ``lib_xs3_math`` library directory.

  * `api/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/api/>`_ - Headers containing the public API for ``lib_xs3_math``.
  * `doc/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/doc/>`_ - Library documentation source (for non-embedded documentation) and build directory.
  * `script/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/script/>`_ - Scripts used for source generation.
  * `src/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/src/>`_ - Library source code.

    * `arch/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/src/arch/>`_ - Architecture specific (Assembly) source files (with generic C implementations).
    * `bfp/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/src/bfp/>`_ - Source files for high-level BFP API.
    * `vect/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/src/vect/>`_- Source files for low-level API.
    * `scalar/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/src/scalar/>`_- Source files for scalar API.

* `/test/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/>`_ - Unit test projects for ``lib_xs3_math``.


Requirements
------------

* xTIMEcomposer tools version 15.0.6 or later
* CMake (for building the library and unit tests)
* GNU Make (for building the libary and unit tests)
* Doxygen (for building documentation)
* Python 3 (for running source generation scripts, if necessary)


API Structure
-------------

The API functions can be categorized in a few ways:

* High-level vs. low-level functions
* 16-bit vs. 32-bit functions
* Arithmetic functions vs. DSP functions

Here we're really only interested in the first of those.

Two-Layer API
*************

``lib_xs3_math`` has a two-layer API. In this model the lower layer (the *low-level API*) acts as the workhorse of the library. It stays close to bare-metal and as such the API may not seem intuitive for anyone not intimately familiar with the XS3's VPU instruction set. By contrast, the upper layer (the *BFP API*) is meant to be intuitive and easy to use, while relying on the low-level API to actually get work done.

The BFP API is a block floating-point API for performing VPU-accelerated operations. In the high-level API each BFP vector is represented by a ``struct`` (such as `bfp_s32_t` for 32-bit BFP vectors). The structure representing a BFP vector carries information about the vector's data buffer and length as well as the vector's exponent and headroom. The BFP functions manage the headroom and exponent of the BFP vectors to determine when and how to best utilize the low-level API to maintain as much arithmetic precision as possible in the data.

The low-level API is implemented primarily in xCORE XS3 Assembly, although C implementations of each function exist. The low-level functions operate directly on arrays of data and require the caller to manage exponents and headroom (although in many cases helper functions exist to do this), and ensure that saturation and underflow are avoided. Sometimes it may be useful to directly use the low-level API rather than the high-level API because the user may know properties of the data that the high-level API cannot simply assume. For example, when subtracting one vector from another, if the user happens to know that both vectors are strictly non-negative, an additional bit of precision may be retained in the result.

All data vectors used by API functions used signed data.

Example
*******

::

    #include "bfp_math.h"

    void foo(const bfp_s32_t* a, const bfp_s32_t* b){
        // BFP vector to store result (although bfp_s32_add() can operate in-place))
        bfp_s32_t sum;

        // Initialize new BFP vector
        bfp_s32_init(&sum, 
            (int32_t*) malloc(a->length * sizeof(int32_t)), // allocate space for data; assume malloc() succeeds
            0,          // exponent of vector; doesn't matter because it will be over-written
            a->length,  // length of vector (in elements)
            0);         // Do not calculate headroom; doesn't matter because it will be over-written

        // Add together the vectors a and b element-wise. Place result in sum.
        bfp_s32_add(&sum, a, b);

        for(int i=0; i < a->length; i++)
            printf("%d:\t%f + %f = %f\n", i, ldexp(a->data[i], a->exp), 
                ldexp(b->data[i], b->exp), ldexp(sum.data[i], sum.exp));

        free(sum.data);
    }


Getting and Building
####################

Clone the Repository
--------------------

To clone this repository you may use the following: ::

    git clone git@github.com:xmos/lib_xs3_math 


Including lib_xs3_math in External Applications
-----------------------------------------------

``lib_xs3_math`` may be included in your own applications either as source to be compiled by your application or as a static library to be linked into your own application. This library uses CMake to manage build configurations. To configure your CMake build environment for ``lib_xs3_math``, from the root of the cloned repository, the following command may be used (ensure that the XTC build tools are on your path): ::

    mkdir build && cd build && cmake -DCMAKE_TOOLCHAIN_FILE=../etc/xmos_toolchain.cmake -G"Unix Makefiles" ..

Then to actually build the the library as a static binary, as well as the unit tests, just use the ``make`` command from the `build` directory.

If you wish to include ``lib_xs3_math`` in your own application as a static library, the generated ``lib_xs3_math.a`` can then be linked into your own application. Be sure to also add ``lib_xs3_math/api`` as an include directory for your project.

To incorporate ``lib_xs3_math`` into your own CMake project, you have two options. You can either add ``/lib_xs3_math`` as a CMake subdirectory (via ``add_subdirectory()``), which will include it as a static library. Or, to include it as a source library you can include ``/lib_xs3_math/lib_xs3_math.cmake`` in your application's CMake project, which will populate various CMake variables (prepended with ``LIB_XS3_MATH_``) with the source files, include directories and build flags required. See ``/lib_xs3_math/lib_xs3_math.cmake`` for the specific variables.

For other build systems

* Add ``lib_xs3_math/api`` as an include directory
* Add all .c files in ``lib_xs3_math/src/vect`` and ``lib_xs3_math/src/bfp`` as source files
* Add all .S files in ``lib_xs3_math/src/arch/xcore`` as source files

  * These are assembly files and should be compiled with ``xcc`` as are the C files.

Then, from your source files, include ``bfp_math.h`` for the BFP API, or ``xs3_math.h`` for the low-level API only.

Unit Tests
----------

This project uses CMake to build the unit test applications. Use the steps described above to configure and build the unit test applications. Both unit test projects currently target the xCORE.ai explorer board. All unit tests are currently in the `/test/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/>`_ directory:

* `/test/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/>`_ - Unit test projects for ``lib_xs3_math``.

  * `bfp_tests/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/bfp_tests/>`_ - High-level BFP API unit test project.
  * `fft_tests/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/fft_tests/>`_ - FFT-related unit tests project.
  * `vect_tests/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/vect_tests/>`_ - Low-level API unit test project.

Low-level Unit Tests
********************

This application runs unit tests for the various 16- and 32-bit low-level vectorized arithmetic functions. This application is located at `/test/vect_tests/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/vect_tests>`_.

To execute the low-level arithmetic unit tests on the explorer board, from your CMake build directory use the following (after ensuring that the hardware is connected and drivers properly installed):

::

    xrun --xscope test/vect_tests/vect_tests.xe

Or, to run the unit tests in the software simulator:

::

    xsim test/vect_tests/vect_tests.xe

Note that running the unit tests in the simulator may be *very* slow.

BFP Unit Tests
**************

This application runs unit tests for the various 16- and 32-bit BFP vectorized arithmetic functions. This application is located at `/test/bfp_tests/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/bfp_tests>`_.

As with ``vect_tests``, the ``bfp_tests`` unit test application can be run with:

::

    xrun --xscope test/bfp_tests/bfp_tests.xe


FFT Unit Tests
**************

This application runs all FFT-related unit tests. This application is located at `/test/fft_tests/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/fft_tests>`_.

As with ``vect_tests``, the ``fft_tests`` unit test application can be run with:

::

    xrun --xscope test/fft_tests/fft_tests.xe


Filter Unit Tests
*****************

This application runs unit tests for 16- and 32-bit FIR filters and 32-bit Biquad filters. This application is located at `/test/filter_tests/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/filter_tests>`_.

As with ``vect_tests``, the ``filter_tests`` unit test application can be run with:

::

    xrun --xscope test/filter_tests/filter_tests.xe


Building Documentation
----------------------

This project currently uses Doxygen for library and API documentation. API functions include embedded documentation with their declarations in their corresponding header files, however, MathJax is used to render equations, and as such, building the stand-alone HTML documentation is highly recommended. In addition, several non-embedded notes and guides are included in the stand-along documentation (these can also be found at `/lib_xs3_math/doc/src/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/doc/>`_ ).

 To build the stand-alone documentation as HTML a Doxygen install will be required. The documentation has been written against Doxygen version 1.8; your mileage may vary with other versions.

With Doxygen on your path, the documentation can be built by calling ``doxygen`` from within the `/lib_xs3_math/doc/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/doc/>`_ directory.

The documentation will be generated within the ``/lib_xs3_math/doc/build/`` directory. To view the HTML version of the documentation, open ``/lib_xs3_math/doc/build/html/index.html`` in a browser.

