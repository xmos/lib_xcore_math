.. _getting_started:

Getting Started
===============

Overview
--------

``lib_xcore_math`` is a library containing efficient implementations of various mathematical 
operations that may be required in an embedded application.  In particular, this library is geared
towards operations which work on vectors or arrays of data, including vectorized arithmetic, 
linear filtering, and fast Fourier transforms.

This library comprises several sub-APIs.  Grouping of operations into sub-APIs is a matter of 
conceptual convenience.  In general, functions from a given API share a common prefix indicating 
which API the function comes from, or the type of object on which it acts.  Additionally, there is
some interdependence between these APIs.

These APIs are:

* :ref:`Block floating-point (BFP) API <bfp_api>` -- High-level API providing operations on BFP
  vectors. See :ref:`bfp_background` for an introduction to block floating-point. These functions
  manage the exponents and headroom of input and output BFP vectors to avoid overflow and underflow 
  conditions.

* :ref:`Vector/Array API <vect_api>` -- Lower-level API which is used heavily by the BFP API. 
  As such, the operations available in this API are similar to those in the BFP API, but the user
  will have to manage exponents and headroom on their own. Many of these routines are implemented
  directly in optimized assembly to use the hardware as efficiently as possible.

* :ref:`Scalar API <scalar_api>` -- Provides various operations on scalar objects. In particular, 
  these operations focus on simple arithmetic operations applied to non-IEEE 754 floating-point 
  objects, as well as optimized operations which are applied to IEEE 754 ``floats``.

* :ref:`Filtering API <filter_api>` -- Provides access to linear filtering operations, including 
  16- and 32-bit FIR filters and 32-bit biquad filters.

* :ref:`Fast Fourier Transform (FFT) API <fft_api>` -- Provides both low-level and block 
  floating-point FFT implementations.  Optimized FFT implementations are provided for real signals,
  pairs of real signals, and for complex signals.

* :ref:`Discrete Cosine Transform (DCT) API <dct_api>` -- Provides functions which implement the 
  `type-II <https://en.wikipedia.org/wiki/Discrete_cosine_transform#DCT-II>`_ ('forward') and 
  `type-III <https://en.wikipedia.org/wiki/Discrete_cosine_transform#DCT-III>`_ ('inverse') DCT for 
  a variety of block lengths. Also provides a fast 8x8 two dimensional forward and inverse DCT.


All APIs are accessed by including the single header file:

.. code-block:: c
  
    #include "xmath/xmath.h"


Building
--------

This library makes use of the CMake build system. See the 
`GitHub page <https://github.com/xmos/lib_xcore_math#readme>`_ for instructions on obtaining this
library's source code and including it in your application's build.

Usage
-----

The following sections are intended to give the reader a general sense of how to use the API.

BFP API
*******

In the BFP API the BFP vectors are C structures such as ``bfp_s16_t``, ``bfp_s32_t``, or
``bfp_complex_s32_t``, backed by a memory buffer. These objects contain a pointer to the data
carrying the content (mantissas) of the vector, as well as information about the length, headroom
and exponent of the BFP vector.

Below is the definition of :c:struct:`bfp_s32_t` from xmath/types.h.

.. code-block:: c

    C_TYPE
    typedef struct {
        /** Pointer to the underlying element buffer.*/
        int32_t* data;
        /** Exponent associated with the vector. */
        exponent_t exp;
        /** Current headroom in the ``data[]`` */
        headroom_t hr;
        /** Current size of ``data[]``, expressed in elements */
        unsigned length;
        /** BFP vector flags. Users should not normally modify these manually. */
        bfp_flags_e flags;
    } bfp_s32_t;

The :ref:`32-bit BFP functions <bfp_s32>` take :c:struct:`bfp_s32_t` pointers as input and output
parameters.

Functions in the BFP API generally are prefixed with ``bfp_``. More specifically, functions where
the 'main' operands are 32-bit BFP vectors are prefixed with ``bfp_s32_``, whereas functions where
the 'main' operands are complex 16-bit BFP vectors are prefixed with ``bfp_complex_s16_``, and so
on for the other BFP vector types.

Initializing BFP Vectors
````````````````````````

Before calling these functions, the BFP vectors represented by the arguments must be initialized.
For :c:struct:`bfp_s32_t` this is accomplished with :c:func:`bfp_s32_init()`.  Initialization
requires that a buffer of sufficient size be provided to store the mantissa vector, as well as an
initial exponent. If the first usage of a BFP vector is as an output, then the exponent will not
matter, but the object must still be initialized before use.  Additionally, the headroom of the
vector may be computed upon initialization; otherwise it is set to ``0``.

Here is an example of a 32-bit BFP vector being initialized.

.. code-block:: c

    #define LEN (20)

    //The object representing the BFP vector
    bfp_s32_t bfp_vect; 

    // buffer backing bfp_vect
    int32_t data_buffer[LEN]; 
    for(int i = 0; i < LEN; i++) data_buffer[i] = i;

    // The initial exponent associated with bfp_vect
    exponent_t initial_exponent = 0; 

    // If non-zero, `bfp_s32_init()` will compute headroom currently present in data_buffer. 
    // Otherwise, headroom is initialized to 0 (which is always safe but may not be optimal)
    unsigned calculate_headroom = 1; 

    // Initialize the vector object
    bfp_s32_init(&bfp_vec, data_buffer, initial_exponent, LEN, calculate_headroom);

    // Go do stuff with bfp_vect
    ...


Once initialized, the exponent and mantissas of the vector can be accessed by ``bfp_vect.exp`` and
``bfp_vect.data[]`` respectively, with the logical (floating-point) value of element ``k`` being
given by :math:`\mathtt{bfp\_vect.data[k]}\cdot2^{\mathtt{bfp\_vect.exp}}`.

BFP Arithmetic Functions
````````````````````````

The following snippet shows a function ``foo()`` which takes 3 BFP vectors, ``a``, ``b`` and ``c``,
as arguments. It multiplies together ``a`` and ``b`` element-wise, and then subtracts ``c`` from the
product. In this example both operations are performed in-place on ``a``. (See
:c:func:`bfp_s32_mul()` and :c:func:`bfp_s32_sub()` for more information about those functions)

.. code-block:: c

    void foo(bfp_s32_t* a, const bfp_s32_t* b, const bfp_s32_t* c)
    {
        // Multiply together a and b, updating a with the result.
        bfp_s32_mul(a, a, b);

        // Subtract c from the product, again updating a with the result.
        bfp_s32_sub(a, a, c);
    }


The caller of ``foo()`` can then access the results through ``a``. Note that the pointer ``a->data``
was not modified during this call.

Vector API
**********

The functions in the lower-level vector API are optimized for performance. They do very little to
protect the user from mangling their data by arithmetic saturation/overflows or underflows (although
they do provide the means to prevent this). 

Functions in the vector API are generally prefixed with ``vect_``. For example, functions which 
operate primarily on 16-bit vectors are prefixed with ``vect_s16_``.  

Some functions are prefixed with ``chunk_`` instead of ``vect_``.  A "chunk" is just a vector with a 
fixed memory footprint (currently 32 bytes, or 8 32-bit elements) meant to match the width of the
architecture's vector registers.

As an example of a function from the vector API, see :c:func:`vect_s32_mul()` (from 
``vect_s32.h``)), which multiplies together two ``int32_t`` vectors element by element.

.. code-block:: c

    C_API
    headroom_t vect_s32_mul(
        int32_t a[],
        const int32_t b[],
        const int32_t c[],
        const unsigned length,
        const right_shift_t b_shr,
        const right_shift_t c_shr);

This function takes two ``int32_t`` arrays, ``b`` and ``c``, as inputs and one ``int32_t`` array,
``a``, as output (in the case of :c:func:`vect_s32_mul()`, it is safe to have ``a`` point to the 
same buffer as ``b`` or ``c``, computing the result in-place). ``length`` indicates the number of 
elements in each array. The final two parameters, ``b_shr`` and ``c_shr``, are the arithmetic
right-shifts applied to each element of ``b`` and ``c`` before they are multiplied together.

Why the right-shifts? In the case of 32-bit multiplication, the largest possible product is 
:math:`2^{62}`, which will not fit in the 32-bit output vector. Applying positive arithmetic 
right-shifts to the input vectors reduces the largest possible product. So, the shifts are there to 
manage the headroom/size of the resulting product in order to maximize precision while avoiding 
overflow or saturation.

Contrast this with :c:func:`vect_s16_mul()`:

.. code-block:: c

    C_API
    headroom_t vect_s16_mul(
        int16_t a[],
        const int16_t b[],
        const int16_t c[],
        const unsigned length,
        const right_shift_t a_shr);

The parameters are similar here, but instead of ``b_shr`` and ``c_shr``, there's only an ``a_shr``.
In this case, the arithmetic right-shift ``a_shr`` is applied to the *products* of ``b`` and ``c``.
In this case the right-shift is also *unsigned* -- it can only be used to reduce the size of the
product.

Shifts like those in these two examples are very common in the vector API, as they are the main
mechanism for managing exponents and headroom.  Whether the shifts are applied to inputs, outputs,
both, or only one input will depend on a number of factors.  In the case of :c:func:`vect_s32_mul()`
they are applied to inputs because the XS3 VPU includes a compulsory (hardware) right-shift of 30 
bits on all products of 32-bit numbers, and so often inputs may need to be *left*-shifted (negative 
shift) in order to avoid underflows.  In the case of :c:func:`vect_s16_mul()`, this is unnecessary
because no compulsory shift is included in 16-bit multiply-accumulates.

Both :c:func:`vect_s32_mul()` and :c:func:`vect_s16_mul()` return the headroom of the output
vector ``a``.

Functions in the vector API are in many cases closely tied to the instruction set architecture
for XS3. As such, if more efficient algorithms are found to perform an operation these low-level API
functions are more likely to change in future versions.