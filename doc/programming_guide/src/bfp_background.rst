.. _bfp_background:

Block Floating-Point Background
===============================

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


Standard floating-point values carry both a mantissa :math:`m` and an exponent :math:`p`, such that
the logical value represented by such a variable is :math:`m\cdot2^p`. When you have a vector of
standard floating-point values, each element of the vector carries its own mantissa and its own
exponent: :math:`m[k]\cdot2^{p[k]}`.

.. image:: images/bfp_bg_fig1.png

By contrast, block floating-point objects have a vector of mantissas :math:`\bar{m}` which all share
the same exponent :math:`p`, such that the logical value of the element at index :math:`k` is
:math:`m[k]\cdot2^p`.

.. code-block:: c

    struct {
        // Array of mantissas
        int32_t mant[20];
        // Shared exponent
        int32_t exp;
    } bfp_vect;

.. image:: images/bfp_bg_fig2.png


.. _headroom_intro:

Headroom
````````

With a given exponent, :math:`p`, the largest value that can be represented by a 32-bit BFP vector
is given by a maximal mantissa (:math:`2^{31}-1`), for a logical value of
:math:`(2^{31}-1)\cdot2^p`. The smallest non-zero value that an element can represent is
:math:`1\cdot2^p`.

Because all elements must share a single exponent, in order to avoid overflow or saturation of the
largest magnitude values, the exponent of a BFP vector is constrained by the element with the
largest (logical) value. The drawback to this is that when the elements of a BFP vector represent a
large dynamic range -- that is, where the largest magnitude element is many, many times larger than
the smallest (non-zero) magnitude element -- the smaller magnitude elements effectively have fewer
bits of precision.

Consider a 2-element BFP vector intended to carry the values :math:`2^{20}` and :math:`255 \cdot
2^{-10}`. One way this vector can be represented is to use an exponent of :math:`0`.

.. code-block:: c

    struct {
        int32_t mant[2];
        int32_t exp;
    } vect = { { (1<<20), (0xFF >> 10) }, 0 };
    
.. image:: images/bfp_bg_fig3.png

In the diagram above, the fractional bits (shown in red text) are discarded, as the mantissa is only
32 bits. Then, with :math:`0` as the exponent, ``mant[1]`` underflows to :math:`0`. Meanwhile, the
12 most significant bits of ``mant[0]`` are all zeros.

The headroom of a signed integer is the number of *redundant* leading sign bits. Equivalently, it is
the number of bits that a mantissa can be left-shifted without losing any information. In the the
diagram, the bits corresponding to headroom are shown in green text. Here ``mant[0]`` has 10 bits of
headroom and ``mant[1]`` has a full 32 bits of headroom. (``mant[0]`` does not have 11 bits of
headroom because in two's complement the MSb serves as a sign bit). The headroom for a BFP vector is
the `minimum` of headroom amongst each of its elements; in this case, 10 bits.

If we remove headroom from one mantissa of a BFP vector, all other mantissas must shift by the same
number of bits, and the vector's exponent must be adjusted accordingly. A left-shift of one bit
corresponds to reducing the exponent by 1, because a single bit left-shift corresponds to 
multiplication by 2.

In this case, if we remove 10 bits of headroom and subtract 10 from the exponent we get the
following:

.. code-block:: c

    struct {
        int32_t mant[2];
        int32_t exp;
    } vect = { { (1<<30), (0xFF >> 0) }, -10 };

.. image:: images/bfp_bg_fig4.png

Now, no information is lost in either element. One of the main goals of BFP arithmetic is to keep
the headroom in BFP vectors to the minimum necessary (equivalently, keeping the exponent as small as
possible). That allows for maximum effective precision of the elements in the vector.

Note that the headroom of a vector also tells you something about the size of the largest magnitude
mantissa in the vector. That information (in conjunction with exponents) can be used to determine
the largest possible output of an operation without having to look at the mantissas.

For this reason, the BFP vectors in ``lib_xcore_math`` carry a field which tracks their current
headroom. The functions in the BFP API use this property to make determinations about how best to
preserve precision.