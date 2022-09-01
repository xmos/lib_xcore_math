
XMath Types
===========

Each of the main operand types used in this library has a short-hand which is used as a prefix in 
the naming of API operations. The following tables can be used for reference.

+--------------------------------------------------------------------------------------------------+
| Common Vector Types                                                                              |
+----------------------+---------------------------------+-----------------------------------------+
| Prefix               | Object Type                     | Notes                                   |
+======================+=================================+=========================================+
| ``vect_s32``         | ``int32_t[]``                   | Raw vector of signed 32-bit integers.   |
+----------------------+---------------------------------+-----------------------------------------+
| ``vect_s16``         | ``int16_t[]``                   | Raw vector of signed 16-bit integers.   |
+----------------------+---------------------------------+-----------------------------------------+
| ``vect_s8``          | ``int8_t[]``                    | Raw vector of signed 8-bit integers.    |
+----------------------+---------------------------------+-----------------------------------------+
| ``vect_complex_s32`` | :c:type:`complex_s32_t`\ ``[]`` | Raw vector of complex 32-bit integers.  |
+----------------------+---------------------------------+-----------------------------------------+
| ``vect_complex_s16`` | (``int16_t[]``, ``int16_t[]``)  | Complex 16-bit vectors are usually      |
|                      |                                 | represented as a pair of 16-bit vectors.|
|                      |                                 | This is an optimization due to the      |
|                      |                                 | word-alignment requirement when loading |
|                      |                                 | data into the VPU's vector registers.   |
+----------------------+---------------------------------+-----------------------------------------+
| ``chunk_s32``        | ``int32_t[8]``                  | A "chunk" is a fixed size vector        |
|                      |                                 | corresponding to the size of the VPU    |
|                      |                                 | vector registers.                       |
+----------------------+---------------------------------+-----------------------------------------+
| ``vect_qXX``         | ``int32_t[]``                   | When used in an API function name, the  |
|                      |                                 | ``XX`` will be an actual number (e.g.   |
|                      |                                 | :c:func:`vect_q30_exp_small()`)         |
|                      |                                 | indicating the fixed-point              |
|                      |                                 | interpretation used by that function.   |
+----------------------+---------------------------------+-----------------------------------------+
| ``vect_f32``         | ``float[]``                     | Raw vector of standard IEEE ``float``   |
+----------------------+---------------------------------+-----------------------------------------+
| ``vect_float_s32``   | :c:type:`float_s32_t`\ ``[]``   | Vector of non-standard 32-bit           |
|                      |                                 | floating-point scalars.                 |
+----------------------+---------------------------------+-----------------------------------------+
| ``bfp_s32``          | :c:type:`bfp_s32_t`             | Block floating-point vector contianing  |
|                      |                                 | 32-bit mantissas.                       |
+----------------------+---------------------------------+-----------------------------------------+
| ``bfp_s16``          | :c:type:`bfp_s16_t`             | Block floating-point vector contianing  |
|                      |                                 | 16-bit mantissas.                       |
+----------------------+---------------------------------+-----------------------------------------+
| ``bfp_complex_s32``  | :c:type:`bfp_complex_s32_t`     | Block floating-point vector contianing  |
|                      |                                 | complex 32-bit mantissas.               |
+----------------------+---------------------------------+-----------------------------------------+
| ``bfp_complex_s16``  | :c:type:`bfp_complex_s16_t`     | Block floating-point vector contianing  |
|                      |                                 | complex 16-bit mantissas.               |
+----------------------+---------------------------------+-----------------------------------------+


+--------------------------------------------------------------------------------------------------+
| Common Scalar Types                                                                              |
+----------------------+---------------------------------+-----------------------------------------+
| Prefix               | Object Type                     | Notes                                   |
+======================+=================================+=========================================+
| ``s32``              | ``int32_t``                     | 32-bit signed integer. May be a simple  |
|                      |                                 | integer, a fixed-point value or the     |
|                      |                                 | mantissa of a floating-point value.     |
+----------------------+---------------------------------+-----------------------------------------+
| ``s16``              | ``int16_t``                     | 16-bit signed integer. May be a simple  |
|                      |                                 | integer, a fixed-point value or the     |
|                      |                                 | mantissa of a floating-point value.     |
+----------------------+---------------------------------+-----------------------------------------+
| ``s8``               | ``int8_t``                      | 8-bit signed integer. May be a simple   |
|                      |                                 | integer, a fixed-point value or the     |
|                      |                                 | mantissa of a floating-point value.     |
+----------------------+---------------------------------+-----------------------------------------+
| ``complex_s32``      | :c:type:`complex_s32_t`         | Signed complex integer with 32-bit real |
|                      |                                 | and 32-bit imaginary parts.             |
+----------------------+---------------------------------+-----------------------------------------+
| ``complex_s16``      | :c:type:`complex_s16_t`         | Signed complex integer with 16-bit real |
|                      |                                 | and 16-bit imaginary parts.             |
+----------------------+---------------------------------+-----------------------------------------+
| ``float_s64``        | :c:type:`float_s64_t`           | Non-standard floating-point scalar with |
|                      |                                 | exponent and 64-bit mantissa.           |
+----------------------+---------------------------------+-----------------------------------------+
| ``float_s32``        | :c:type:`float_s32_t`           | Non-standard floating-point scalar with |
|                      |                                 | exponent and 32-bit mantissa.           |
+----------------------+---------------------------------+-----------------------------------------+
| ``qXX``              | ``int32_t``                     | 32-bit fixed-point value with ``XX``    |
|                      |                                 | fractional bits (i.e. exponent of       |
|                      |                                 | ``-XX``).                               |
+----------------------+---------------------------------+-----------------------------------------+
| ``f32``              | ``float``                       | Standard IEEE 754 single-precision      |
|                      |                                 | ``float``.                              |
+----------------------+---------------------------------+-----------------------------------------+
| ``f64``              | ``double``                      | Standard IEEE 754 double-precision      |
|                      |                                 | ``float``.                              |
+----------------------+---------------------------------+-----------------------------------------+
| ``float_complex_s64``| :c:type:`float_complex_s64_t`   | Floating-point value with exponent and  |
|                      |                                 | complex mantissa with 64-bit real and   |
|                      |                                 | imaginary parts.                        |
+----------------------+---------------------------------+-----------------------------------------+
| ``float_complex_s32``| :c:type:`float_complex_s32_t`   | Floating-point value with exponent and  |
|                      |                                 | complex mantissa with 32-bit real and   |
|                      |                                 | imaginary parts.                        |
+----------------------+---------------------------------+-----------------------------------------+
| ``float_complex_s16``| :c:type:`float_complex_s16_t`   | Floating-point value with exponent and  |
|                      |                                 | complex mantissa with 16-bit real and   |
|                      |                                 | imaginary parts.                        |
+----------------------+---------------------------------+-----------------------------------------+
| N/A                  | :c:type:`exponent_t`            | Represents an exponent :math:`p` as in  |
|                      |                                 | :math:`2^p`. Unless otherwise specified |
|                      |                                 | exponent are always assumed to have a   |
|                      |                                 | base of :math:`2`.                      |
+----------------------+---------------------------------+-----------------------------------------+
| N/A                  | :c:type:`headroom_t`            | The headroom of a scalar or vector.     |
|                      |                                 | See :ref:`headroom_intro` for more      |
|                      |                                 | information.                            |
+----------------------+---------------------------------+-----------------------------------------+
| N/A                  | :c:type:`right_shift_t`         | Represents a rightward bit-shift of a   |
|                      |                                 | certain number of bits. Care should be  |
|                      |                                 | taken, as sometimes this is treated as  |
|                      |                                 | unsigned.                               |
+----------------------+---------------------------------+-----------------------------------------+
| N/A                  | :c:type:`left_shift_t`          | Represents a leftward bit-shift of a    |
|                      |                                 | certain number of bits. Care should be  |
|                      |                                 | taken, as sometimes this is treated as  |
|                      |                                 | unsigned.                               |
+----------------------+---------------------------------+-----------------------------------------+
 

Block Floating-Point Types
--------------------------

.. doxygengroup:: type_bfp
  :members:
  :content-only:


Scalar Types (Integer)
----------------------

.. doxygengroup:: type_scalar_int
    :members:
    :content-only:

Scalar Types (Floating-Point)
-----------------------------

.. doxygengroup:: type_scalar_float
    :members:
    :content-only:


Scalar Types (Fixed-Point)
--------------------------

.. doxygengroup:: type_scalar_fixed
    :members:
    :content-only:


Misc Types
----------

.. doxygengroup:: type_misc
  :members:
  :content-only:
 