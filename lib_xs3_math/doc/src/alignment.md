
Note on Vector Alignment                   {#alignment}
========================

This library utilizes the XMOS XS3 architecture's vector processor unit (VPU). All loads and stores to and from the XS3 VPU have the requirement that the loaded/stored addresses must be aligned to a 4-byte boundary (word-aligned).

In the current version of the API, this leads to the requirement that most API functions require vectors to begin at word-aligned addresses. Vectors are *not* required, however to have a length (in bytes) that is a multiple of 4.

#### Writing Alignment-safe Code ####

Arrays of type `int32_t` and `complex_s32_t` will normally be guaranteed to be word-aligned by the compiler. However, if the user manually specifies the beginning of an `int32_t` array, as in the following..

\code{.c}
    uint8_t byte_buffer[100];
    int32_t* integer_array = (int32_t*) &byte_buffer[1];
\endcode

.. it is the responsibility of the user to ensure proper alignment of data.

For `int16_t` arrays, the compiler does not by default guarantee that the array starts on a word-aligned address. To
force word-alignment on this type, use `__attribute__((align 4))` in the variable definition, as in the following.

\code{.c}
    int16_t __attribute__((align 4)) data[100];
\endcode
