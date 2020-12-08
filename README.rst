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

* `<https://github.com/xmos/lib_xs3_math>`_ - The actual ``lib_xs3_math`` library directory.

  * `api/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/api/>`_ - Headers containing the public API for ``lib_xs3_math``.
  * `doc/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/doc/>`_ - Library documentation source (for non-embedded documentation) and build directory.
  * `script/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/script/>`_ - Scripts used for source generation.
  * `src/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/src/>`_ - Library source code.

    * `arch/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/src/arch/>`_ - Architecture specific (Assembly) source files.
    * `high/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/src/high/>`_ - Source files for high-level API.
    * `low/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/src/low/>`_- Source files for low-level API.

* `/test/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/>`_ - Unit test projects for ``lib_xs3_math``.

  * `fft_test/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/fft_test/>`_ - FFT unit tests project.
  * `shared/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/shared/>`_ - Some files shared by both unit test projects.
  * `unit_test/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/unit_test/>`_ - Vectorized arithmetic unit test project.


Requirements
------------

* xTIMEcomposer tools version 15.0.1 or later
* The XMOS fork of the Unity unit test framework (for building unit tests)
* Doxygen (for building documentation)
* Python 3 (for auto-generation of source files)


API Structure
-------------

The API functions can be categorized in a few ways:

* High-level vs. low-level functions
* 16-bit vs. 32-bit functions
* Arithmetic functions vs. DSP functions

Here we're really only interested in the first of those.

Two-Layer API
*************

``lib_xs3_math`` has a two-layer API. In this model the lower layer (the *low-level API*) acts as the workhorse of the library. It stays close to bare-metal and as such the API may not seem intuitive for those not intimately familiar with the XS3's VPU instruction set. By contrast, the upper layer (the *high-level API*) in this model is meant to be intuitive and easy to use, while relying on the low-level API to actually get the work done.

The high-level API is a block floating-point (BFP) API for performing VPU-accelerated operations. In the high-level API each BFP vector is represented by a ``struct``. The structure representing a BFP vector carries information about the vector's data buffer and length as well as the vector's exponent and headroom. The high-level API functions manages the headroom and exponent of the BFP vectors to determine when and how to best utilize the low-level API to maintain as much arithmetic precision as possible in the data.

The low-level API is implemented primarily in xCORE XS3 Assembly, although C implementations of each function exist. The low-level functions operate directly on arrays of data and require the caller to manage exponents and headroom, and ensure that saturation and underflow are avoided. Sometimes it may be useful to directly use the low-level API rather than the high-level API because the user may know properties of the data that the high-level API cannot simply assume. For example, when subtracting one vector from another, if the user happens to know that both vectors are strictly non-negative, an additional bit of precision may be retained in the result.


* Auto-generated source files

Example
*******

::

    #include "bfp_math.h"

    void foo(const bfp_s32_t* a, const bfp_s32_t* b){
        // BFP vector to store result (although bfp_s32_add() can operate in-place))
        bfp_s32_t sum;

        // Initialize new BFP vector
        bfp_s32_init(&sum, 
            (int32_t*) malloc(a->length * sizeof(int32_t)), // allocate space for data; assume malloc succeeds()
            0,          // exponent of vector; doesn't matter because it will be over-written
            a->length,  // length of vector (in elements)
            0);         // Do not calculate headroom; doesn't matter because it will be over-written

        // Add together the vectors a and b element-wise. Place result in sum.
        bfp_s32_add(&sum, &a, &b);

        for(int i=0; i<a->length; i++)
            printf("%d:\t%f + %f = %f\n", i, ldexp(a->data[i], a->exp), 
                ldexp(b->data[i], b->exp), ldexp(sum->data[i], sum->exp));

        free(sum->data);
    }


Getting and Building
####################

Clone the Repository
--------------------

To clone this repository and its submodules (Unity repo required for unit tests) use the following: ::

    git clone --recurse-submodules git@github.com:xmos/lib_xs3_math 


Including lib_xs3_math in External Applications
-----------------------------------------------

To include ``lib_xs3_math`` in your application

* add `/lib_xs3_math/api/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/api/>`_ as an include directory.
* add the ``.c`` and ``.S`` files in `/lib_xs3_math/src <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math>`_ to your source.

Some build-time configuration of the library is possible by using certain global defines. See `xs3_math_conf.h <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/api/xs3_math_conf.h>`_ and its associated documentation for more information.

The Make script used in the unit test apps auto-generates a couple source files (``xs3_fft_lut.c`` and ``xs3_fft_lut.h`` -- supports various max FFT sizes without wasting memory) in the build directory. If your project has no need to auto-generate those files (because you know your maximum FFT length), use `this Python script <https://github.com/xmos/lib_xs3_math/blob/develop/lib_xs3_math/script/gen_fft_table.py>`_ to generate them once and include them in your own project. Note that ``xs3_fft_lut.h`` needs to be in the include path for the ``lib_xs3_math`` source.

From user code, ``lib_xs3_math`` API functions can be accessed by including `xs3_math.h <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/api/xs3_math.h>`_.

Unit Tests
----------

This project uses GNU Make to build the unit test applications. Both unit test projects currently target the xCORE.ai explorer board.

Arithmetic Unit Tests
*********************

This application runs unit tests for the various 16- and 32-bit vectorized arithmetic functions. This application is located at `/test/unit_test/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/unit_test>`_.

::

    cd test/unit_test

To build the application, use ``make``:

::

    make all

To run the unit tests on the explorer board (after ensuring that the hardware is connected and drivers properly installed):

::

    xrun --io bin/xcore/unit_test.xe

To run the unit tests in the software simulator:

::

    xsim bin/xcore/unit_test.xe

Note that running the unit tests in the simulator may be *very* slow. To run more quickly (by reducing the number of iterations of certain pseudorandom tests), the ``QUICK_TEST=1`` option can be used:

::

    make all QUICK_TEST=1

Assembly implementations of low-level functions will be preferred for xCORE where they exist. All low-level API functions written in xCORE Assembly have pure C counterparts which (*should*) give identical results. When debugging a problem, it may sometimes be useful to use the C implementations instead. To avoid compiling the ``.S`` Assembly files in ``lib_xs3_math``, the ``NO_ASM=1`` option can be used:

::

    make all NO_ASM=1

The unit tests can also be compiled to run on the host system by using the ``PLATFORM=x86`` option:

::

    make all PLATFORM=x86

which will leave a binary in ``bin/x86/``.


FFT Unit Tests
**************

This application runs all unit tests associated with the behavior of the library's FFT. This application is located at `/test/fft_test/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/>`_.

::

    cd test/fft_test

To build the application, use ``make``:

::

    make all

To run the FFT unit tests on the explorer board (after ensuring that the hardware is connected and drivers properly installed):

::

    xrun --io bin/xcore/fft_test.xe

To run the FFT unit tests in the software simulator:

::

    xsim bin/xcore/fft_test.xe

Note that running the FFT unit tests in the simulator may be *very* slow. To run more quickly (by reducing the number of iterations of certain pseudorandom tests), the ``QUICK_TEST=1`` option can be used:

::

    make all QUICK_TEST=1

Assembly implementations of low-level functions will be preferred for xCORE where they exist. All low-level API functions written in xCORE Assembly have pure C counterparts which (*should*) give identical results. When debugging a problem, it may sometimes be useful to use the C implementations instead. To avoid compiling the ``.S`` Assembly files in ``lib_xs3_math``, the ``NO_ASM=1`` option can be used:

::

    make all NO_ASM=1

The FFT unit tests can also be compiled to run on the host system by using the ``PLATFORM=x86`` option:

::

    make all PLATFORM=x86

which will leave a binary in ``bin/x86/``.



Building Documentation
----------------------

This project currently uses Doxygen for library and API documentation. As such, a Doxygen install will be required to 
build the documentation. The documentation has been written against Doxygen version 1.8; your mileage may vary with
other versions.

With Doxygen on your path, the documentation can be built either by invoking the ``docs`` Make target from either unit test application's directory.

::

    make docs

Alternatively, by calling ``doxygen`` from within the `</lib_xs3_math/doc/` directory.

The documentation will be generated within the ``/lib_xs3_math/doc/build/`` directory. To view the HTML version of the documentation, open ``/lib_xs3_math/doc/build/html/index.html`` in a browser.

