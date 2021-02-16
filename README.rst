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

* `/test/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/>`_ - Unit test projects for ``lib_xs3_math``.


Requirements
------------

* xTIMEcomposer tools version 15.0.1 or later
* The XMOS fork of the Unity unit test framework (for building unit tests)

  * This is included as a sub-module when cloning this repository

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

To clone this repository and its submodules (Unity repo required for unit tests) use the following: ::

    git clone --recurse-submodules git@github.com:xmos/lib_xs3_math 


Including lib_xs3_math in External Applications
-----------------------------------------------

``lib_xs3_math`` is intended to be compiled into a static library which is linked into your own application. Once the repository and its submodules have been cloned, ``make build`` can be run from the root of the cloned directory to build the ``lib_xs3_math`` static library and all unit tests.

To build documentation, ``make docs`` can be run from that same directory.

To use ``lib_xs3_math`` in your own xCore application, the static library ``lib_xs3_math.a`` (found at ``lib_xs3_math/lib/xcore/``) can then be linked into your application. The ``lib_xs3_math.a`` found at ``lib_xs3_math/lib/ref/`` uses the un-optimized implementations of the low-level functions. `/lib_xs3_math/api/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/api/>`_ should be added as an include directory to your own project.

Then, from your source files, include ``bfp_math.h`` for the BFP API, or ``xs3_math.h`` for the low-level API only.

To include ``lib_xs3_math`` in your application

* add `/lib_xs3_math/api/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/api/>`_ as an include directory.
* add the ``.c`` and ``.S`` files in `/lib_xs3_math/src <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math>`_ to your source.

Some build-time configuration of the library is possible by using certain global defines. See `xs3_math_conf.h <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/api/xs3_math_conf.h>`_ and its associated documentation for more information. 

If you prefer to use your own build system to build ``lib_xs3_math``, include all source files in `/lib_xs3_math/src/vect/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/src/vect/>`_ and `/lib_xs3_math/src/bfp/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/src/bfp/>`_, and all source files in *one subdirectory* of `/lib_xs3_math/src/arch/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/src/arch/>`_. If prototyping an algorithm on a host system, rather than on xCore, use `/lib_xs3_math/src/arch/ref/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/src/arch/ref/>`_, otherwise `/lib_xs3_math/src/arch/xcore/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/src/arch/xcore/>`_ should be compiled.

Unit Tests
----------

This project uses GNU Make to build the unit test applications. Both unit test projects currently target the xCORE.ai explorer board. All unit tests are currently in the `/test/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/>`_ directory:

* `/test/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/>`_ - Unit test projects for ``lib_xs3_math``.

  * `bfp_tests/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/bfp_tests/>`_ - High-level BFP API unit test project.
  * `fft_tests/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/fft_tests/>`_ - FFT-related unit tests project.
  * `vect_tests/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/vect_tests/>`_ - Low-level API unit test project.


Fetching Dependencies
*********************

Run the following commands to fetch all dependencies that are required to build the unit tests.

Move to test directory:

::

    cd test

Fetch the dependencies:

::

    ./fetch_dependencies.py

Low-level Unit Tests
********************

This application runs unit tests for the various 16- and 32-bit low-level vectorized arithmetic functions. This application is located at `/test/vect_tests/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/vect_tests>`_.

Move to test directory:

::

    cd test/vect_tests

Build the unit test application:

::

    make all

To run the unit tests on the explorer board (after ensuring that the hardware is connected and drivers properly installed):

::

    xrun --xscope bin/xcore/vect_tests.xcore.xe

To run the unit tests in the software simulator:

::

    xsim bin/xcore/vect_tests.xcore.xe

Note that running the unit tests in the simulator may be *very* slow.

BFP Unit Tests
**************

This application runs unit tests for the various 16- and 32-bit BFP vectorized arithmetic functions. This application is located at `/test/bfp_tests/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/bfp_tests>`_.

Move to test directory:

::

    cd test/bfp_tests

Build the unit test application:

::

    make all

To run the unit tests on the explorer board (after ensuring that the hardware is connected and drivers properly installed):

::

    xrun --xscope bin/xcore/bfp_tests.xcore.xe

To run the unit tests in the software simulator:

::

    xsim bin/xcore/bfp_tests.xcore.xe

Note that running the unit tests in the simulator may be *very* slow.


FFT Unit Tests
**************

This application runs all FFT-related unit tests. This application is located at `/test/fft_tests/ <https://github.com/xmos/lib_xs3_math/tree/develop/test/fft_tests>`_.

Move to test directory:

::

    cd test/fft_tests

Build the unit test application:

::

    make all

To run the FFT unit tests on the explorer board (after ensuring that the hardware is connected and drivers properly installed):

::

    xrun --xscope bin/xcore/fft_tests.xcore.xe

To run the unit tests in the software simulator:

::

    xsim bin/xcore/fft_tests.xcore.xe

Note that running the unit tests in the simulator may be *very* slow.


Building Documentation
----------------------

This project currently uses Doxygen for library and API documentation. API functions include embedded documentation with their declarations in their corresponding header files, however, MathJax is used to render equations, and as such, building the stand-alone HTML documentation is highly recommended. In addition, several non-embedded notes and guides are included in the stand-along documentation (these can also be found at `/lib_xs3_math/doc/src/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/doc/>`_ ).

 To build the stand-alone documentation as HTML a Doxygen install will be required. The documentation has been written against Doxygen version 1.8; your mileage may vary with other versions.

With Doxygen on your path, the documentation can be built by invoking the ``docs`` Make target from any directory containing a ``Makefile``.

Alternatively, the documentaiton can be built by calling ``doxygen`` from within the `/lib_xs3_math/doc/ <https://github.com/xmos/lib_xs3_math/tree/develop/lib_xs3_math/doc/>`_ directory.

The documentation will be generated within the ``/lib_xs3_math/doc/build/`` directory. To view the HTML version of the documentation, open ``/lib_xs3_math/doc/build/html/index.html`` in a browser.

