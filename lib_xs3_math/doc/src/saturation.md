
Note on Symmetrically Saturating BFP Arithmetic         {#saturation}
===============================================

With ordinary integer arithmetic the block floating-point logic chooses exponents and operand shifts to prevent integer overflow with worst-case input values. However, the XS3 VPU utilizes symetrically saturating integer arithmetic.

Saturating arithmetic is that where partial results of the applied operation use a bit depth greater than the output bit depth, and values that can't be properly expressed with the output bit depth are set to the nearest expressible value. 

For example, in ordinary C integer arithmetic, a function which multiplies two 32-bit integers may internally compute the full 64-bit product and then clamp values to the range `(INT32_MIN, INT32_MAX)` before returning a 32-bit result.

Symmetrically saturating arithmetic also includes the property that the lower bound of the expressible range is the negative of the upper bound of the expressible range.

One of the major troubles with non-saturating integer arithmetic is that in a twos complement encoding, there exists an integer (e.g. INT16_MIN in 16-bit twos complement arithmetic) value @f$x@f$ for which  @f$-1 \cdot x = x@f$. Serious arithmetic errors can result when this case is not accounted for.

One of the results of _symmetric_ saturation, on the other hand, is that there is a corner case where (using the same exponent and shift logic as non-saturating arithmetic) saturation may occur for a particular combination of input mantissas. The corner case is different for different operations.

When the corner case occurs, the minimum (and largest magnitude) value of the resulting vector is 1 LSb greater than its ideal value (e.g. `-0x3FFF` instead of `-0x4000` for 16-bit arithmetic). The error in this output element's mantissa is then 1 LSb, or @f$2^p@f$, where @f$p@f$ is the exponent of the resulting BFP vector.

Of course, the very nature of BFP arithmetic routinely involves errors of this magnitude. However, this particular case may have pitfalls that don't occur when using asymmetric BFP arithmetic.

The solution to this problem is to detect the possibility of this corner case and adjust parameters to avoid it when it is possible. To that end, this library has a compile time option, `XS3_BFP_ALLOW_SATURATION` (which defaults to `0`) to specify whether the BFP API functions should globally detect and avoid this corner case. 

If there are specific operations where a different local behavior is desired, most of the BFP operations have a function (e.g. `bfp_add_vect_calc_params()` for vector addition) which can be used in combination with the low-level API to specify local behavior at run-time. The low-level API requires the user to manually specify the shift values used as its operations are carried out, and is thus agnostic to (and has no visibility of) the setting for `XS3_BFP_ALLOW_SATURATION`.

Note that the logic which regards `XS3_BFP_ALLOW_SATURATION` only checks for the _possibility_ of reaching the saturating corner case, rather than whether it will _actually_ occur. Detecting the actuality of this corner case is much more expensive.

The down side to disallowing corner-case saturation is that whenever the possibility of saturation is detected the effective bit depth of the resulting BFP vector is reduced by 1 bit. How frequently the possibility of saturation is detected depends in general both on the operation being performed and on the actual logical content of the vectors. For example, with BFP vector addition, adding vectors which are unlikely to be of the same magnitude (as determined by the value represented by their respective largest magnitude elements) detection of the saturation condition will be infrequent.
