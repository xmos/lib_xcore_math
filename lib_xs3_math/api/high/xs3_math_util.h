

#ifndef XS3_MATH_UTIL_H_
#define XS3_MATH_UTIL_H_

// Note: These are not safe from multiple evaluation of arguments

#define MAX(A,B) (((A) >= (B))? (A) : (B))
#define MIN(A,B) (((A) <= (B))? (A) : (B))

// #define MAX(A,B)   ({ __typeof__ (A) _A = (A); __typeof__ (B) _B = (B); _A >= _B ? _A : _B; })
// #define MIN(A,B)   ({ __typeof__ (A) _A = (A); __typeof__ (B) _B = (B); _A <= _B ? _A : _B; })

/**
 * Perform an arithmetic right-shift with a signed shift. i.e. negative values of `SHR` will left-shift.
 */ 
#define SIGNED_ASHR(VAL, SHR)  (((SHR) >= 0)? ((VAL) >> (SHR)) : ((VAL) << (-(SHR))))

#endif //XS3_MATH_UTIL_H_