
Getting Started
===============

@tableofcontents

Overview
--------

``lib_xs3_math`` has a two layer API model. The upper layer is a block floating-point (BFP) API in which many details of operations being performed are hidden from the user. The lower layer, called the `low-level API`, stays much closer to the metal and requires that some care be taken to avoid conditions like arithmetic saturation or underflow. The BFP API calls the low-level API, which is where the bulk of the work is actually performed.


Background
----------

Block Floating-Point Vectors
````````````````````````````

A standard (IEEE) floating-point object can exist either as a scalar, e.g.


.. code-block:: c

    //Single IEEE floating-point variable
    float foo;


or as a vector, e.g.

.. code-block:: c

    //Array of IEEE floating-point variables
    float foo[20];


Standard floating-point values carry both a mantissa :math:`m` and an exponent :math:`p`, such that the logical value represented by such a variable is :math:`m\cdot2^p`. When you have a vector of standard floating-point values, each element of the vector carries its own mantissa and its own exponent: :math:`m[k]\cdot2^{p[k]`

::

    FIGURE WENT HERE

By contrast, block floating-point objects have a vector of mantissas :math:`\bar{m}` which all share the same exponent :math:`p`, such that the logical value of the element at index :math:`k` is :math:`m[k]\cdot2^p`.

.. code-block:: c

    struct {
        // Array of mantissas
        int32_t mant[20];
        // Shared exponent
        int32_t exp;
    } bfp_vect;

    ::

    FIGURE WENT HERE

Headroom
````````

With a given exponent, :math:`p`, the largest value that can be represented by a 32-bit BFP vector is given by a maximal mantissa (:math:`2^{31}-1`), for a logical value of :math:`(2^{31}-1)\cdot2^p`. The smallest non-zero value that an element can represent is :math:`1\cdot2^p`.

Because all elements must share a single exponent, in order to avoid overflow or saturation of the largest magnitude values, the exponent of a BFP vector is constrained by the element with the largest (logical) value. The drawback to this is that when the elements of a BFP vector represent a large dynamic range -- that is, where the largest magnitude element is many, many times larger than the smallest (non-zero) magnitude element -- the smaller magnitude elements effectively have fewer bits of precision.

Consider a 2-element BFP vector intended to carry the values :math:`2^{20}` and :math:`255 \cdot 2^{-10}`. One way this vector can be represented is to use an exponent of :math:`0`.

.. code-block:: c

    struct {
        int32_t mant[2];
        int32_t exp;
    } vect = { { (1<<20), (0xFF >> 10) }, 0 };
    
::

    FIGURE WENT HERE

In the diagram above, the fractional bits (shown in red text) are discarded, as the mantissa is only 32 bits. Then, with :math:`0` as the exponent, ``mant[1]`` underflows to :math:`0`. Meanwhile, the 12 most significant bits of ``mant[0]`` are all zeros.

Headroom is the number of bits that a mantissa can be left-shifted without losing any information. In the the diagram, the bits corresponding to headroom are shown in green text. Here ``mant[0]`` has 10 bits of headroom and ``mant[1]`` has a full 32 bits of headroom. (``mant[0]`` does not have 11 bits of headroom because in two's complement the MSb serves as a sign bit). The headroom for a BFP vector is the `minimum` of headroom amongst each of its elements; in this case, 10 bits.

If we remove headroom from one mantissa of a BFP vector, all other mantissas must shift by the same number of bits, and the vector's exponent must be adjusted accordingly. A left-shift of one bit corresponds to reducing the exponent by 1.

In this case, if we remove 10 bits of headroom and subtract 10 from the exponent we get the following:

::

    FIGURE WENT HERE

Now, no information is lost in either element. One of the main goals of BFP arithmetic is to keep the headroom in BFP vectors to the minimum necessary (equivalently, keeping the exponent as small as possible). That allows for maximum effective precision of the elements in the vector.

Note that the headroom of a vector also tells you something about the size of the largest magnitude mantissa in the vector. That information (in conjunction with exponents) can be used to determine the largest possible output of an operation without having to look at the mantissas.

For this reason, the BFP vectors in ``lib_xs3_math`` carry a field which tracks their current headroom. The BFP functions in the high-level API use this property to make determinations about how best to preserve precision.

BFP API
-------

In the high-level API the BFP vectors C structures such as ``bfp_s16_t``, ``bfp_s32_t``, or ``bfp_complex_s32_t``, backed by a memory buffer. These objects contain a pointer to the data carrying the content (mantissas) of the vector, as well as information about the length, headroom and exponent of the BFP vector.

Below is the definition of ``bfp_s32_t`` from xs3_math_types.h.

.. @snippet{lineno} api/xs3_math_types.h bfp_s32_t 

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

The functions in @ref bfp_vector_functions32 take ``bfp_s32_t`` references as input and output parameters.

Functions in the BFP API generally are prefixed with ``bfp_``.

Initializing BFP Vectors
````````````````````````

Before calling these functions, the BFP vectors represented by the arguments must be initialized. For ``bfp_s32_t`` this 
is accomplished with ``bfp_s32_init()``.

.. code-block:: c

    #define LEN (20)

    //The object representing the BFP vector
    bfp_s32_t bfp_vect; 

    // buffer backing bfp_vect
    int32_t data_buffer[LEN]; 
    for(int i = 0; i < LEN; i++) data_buffer[i] = i;

    // The initial exponent associated with bfp_vect
    exponent_t initial_exponent = 0; 

    // If non-zero, ``bfp_s32_init()`` will compute the headroom currently present in data_buffer. 
    // Otherwise, headroom is initialized to 0 (which is always safe but may not be optimal)
    unsigned calculate_headroom = 1; 

    // Initialize the vector object
    bfp_s32_init(&bfp_vec, data_buffer, initial_exponent, LEN, calculate_headroom);

    // Go do stuff with bfp_vect
    ...


Once initialized, the exponent and mantissas of the vector can be accessed by ``bfp_vect->exp`` and ``bfp_vect->data[]`` respectively, with the logical (floating-point) value of element ``k`` being given by ``ldexp(bfp_vect->data[k], bfp_vect->exp)``.

BFP Arithmetic Functions
````````````````````````

The following snippet shows a function ``foo()`` which takes 3 BFP vectors, ``a``, ``b`` and ``c``, as arguments. It multiplies together ``a`` and ``b`` element-wise, and then subtracts ``c`` from the product. In this example both operations are performed in-place on ``a``. (See ``bfp_s32_mul()`` and ``bfp_s32_sub()`` for more information about those functions)

.. code-block:: c

    void foo(bfp_s32_t* a, const bfp_s32_t* b, const bfp_s32_t* c)
    {
        // Multiply together a and b, updating a with the result.
        bfp_s32_mul(a, a, b);

        // Subtract c from the product, again updating a with the result.
        bfp_s32_sub(a, a, c);
    }



The caller of ``foo()`` can then access the results through ``a``. Note that the pointer ``a->data`` was not modified during this call.

Low-level API
-------------

The functions in the low-level API are optimized for performance. They do very little to protect the user from mangling their data by arithmetic saturation/overflows or underflows. Functions in the low-level API are generally prefixed with ``xs3_``.

As an example of a function from the low-level API, see ``xs3_vect_s32_mul()`` from ``xs3_bfp_low.h``, which multiplies together two ``int32_t`` vectors element by element.

.. @snippet{lineno} api/vect/xs3_vect_s32.h xs3_vect_s32_mul

.. code-block:: c

    C_API
    headroom_t xs3_vect_s32_mul(
        int32_t a[],
        const int32_t b[],
        const int32_t c[],
        const unsigned length,
        const right_shift_t b_shr,
        const right_shift_t c_shr);

This function takes two ``int32_t`` arrays, ``b`` and ``c``, as inputs and one ``int32_t`` array, ``a``, as output. ``length`` indicates the number of elements in each array. The final two parameters, ``b_shr`` and ``c_shr``, are the arithmetic right-shifts applied to each element of ``b`` and ``c`` before they are multiplied together.

Why the right-shifts? This reflects details of the XS3 instructions which target the VPU. With the XS3 VPU, multiplications of 32-bit numbers always include a compulsory (rounding) right-shift by 30 bits. So, to multiply two vectors element-wise with managed precision, the inputs must be shifted before multiplication to ensure the results are scaled as desired.

Contrast this with ``xs3_vect_s16_mul()``:

.. @snippet{lineno} api/vect/xs3_vect_s16.h xs3_vect_s16_mul

.. code-block:: c

    C_API
    headroom_t xs3_vect_s16_mul(
        int16_t a[],
        const int16_t b[],
        const int16_t c[],
        const unsigned length,
        const right_shift_t a_shr);

The parameters are similar here, but instead of ``b_shr`` and ``c_shr``, there's only an ``a_shr``. This reflects the fact that products of 16-bit numbers can be accumulated without a compulsory right-shift, and so there is no risk of losing information by multiplying. Instead, a single right-shift can be applied to the 32-bit product to correctly scale theresult.

Both ``xs3_vect_s32_mul()`` and ``xs3_vect_s16_mul()`` return the headroom of the output vector ``a``.

Functions in the low-level API are in many cases closely tied to the instruction set architecture for XS3. As such, when more efficient algorithms are found to perform an operation these functions are more likely to change.