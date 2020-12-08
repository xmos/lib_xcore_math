
# Getting Started #         {#getting_started}

@tableofcontents

## Overview ##

`lib_xs3_math` has a two layer API model. The upper layer, referred to as the _high-level API_ is a block floating-point 
API in which many details of operations being performed are hidden from the user. The lower layer, called the _low-level 
API_, stays much closer to the metal and requires that some care be taken to avoid conditions like arithmetic saturation
or underflow. The high-level API calls the low-level API, which is where the bulk of the work is actually performed.

@htmlonly
<div style="text-align:center">
<svg width="400" height="400">
    <defs>
        <marker id="arrowhead" markerWidth="20" markerHeight="20" 
                refX="1" refY="10" stroke="black" fill="black"
                orient="auto" markerUnits="userSpaceOnUse">
            <polygon points="0 0, 20 10, 0 20" /> </marker> </defs>
    <style>
    text {fill:black; }
    g.layer > rect { fill:gray; rx:15; ry:15; stroke:black; stroke-width:3; width:100%; height:100%; }
    g.layer > text {fill:black; dominant-baseline:middle; text-anchor:start; font-size:120%; }
    svg.downarrow > line {stroke:black; stroke-width:15; marker-end:url(#arrowhead); }
    svg.downarrow > text { dominant-baseline:middle; text-anchor:middle; fill:white; transform:rotate(90deg)}
    svg.layer-contents > rect { stroke-dasharray:7 3; fill:#999; stroke:black; rx:10; ry:10; width:80%; height:80%;  }
    svg.layer-contents > text { dominant-baseline:hanging; }
    #layer-high > g.layer > rect { fill:#777; }
    #layer-low  > g.layer > rect { fill:#555; }
    #layer-xs3  > g.layer > rect { fill:#333; }
    #xs3-math-box { fill:#158; rx:5; ry:5; height:80%; width:10%; stroke:black; stroke-dasharray:10 10; }
    #xs3-math-label { font-size:2.5em; dominant-baseline:hanging; text-anchor:middle; transform:rotate(-90deg); }
    </style>
    <g transform="translate(1,1) scale(0.95)">
    <rect id="xs3-math-box"/>
    <text id="xs3-math-label" x="-40%" y="1%" >lib_xs3_math</text>
    <svg x="10%" width="90%" y="0%" height="40%" id="layer-high">
        <g class="layer"> 
            <rect /> <text x="20%" y="50%">High-level API</text> </g>
        <svg class="layer-contents" x="55%" y="20%" width="40%" height="60%">
            <rect x="0%" y="0%" /> <rect x="10%" y="10%" /> <rect x="20%" y="20%" />
            <text y="30%">
                <tspan x="30%" dy="10">BFP functions</tspan>
                <tspan x="30%" dy="20">written in C </tspan> </text> </svg> </svg>
    <svg x="10%" width="90%" y="40%" height="40%" id="layer-low">
        <g class="layer"> 
            <rect /> <text x="20%" y="50%" class="label">Low-level API</text> </g>
        <svg class="layer-contents" x="55%" y="20%" width="40%" height="60%">
            <rect x="0%" y="0%" /> <rect x="10%" y="10%" /> <rect x="20%" y="20%" />
            <text y="30%">
                <tspan x="30%" dy="10">VPU Functions</tspan>
                <tspan x="30%" dy="20">written in ASM</tspan></text></svg></svg>
    <svg x="10%" width="90%" y="80%" height="20%" id="layer-xs3">
        <g class="layer"> <rect /> <text x="30%" y="50%">xCORE XS3 Hardware</text> </g></svg>
    <svg x="10%" y="27%" width="100" height="100" class="downarrow">
        <line x1="50%" y1="0%" x2="50%" y2="80%" />
        <text x="40%" y="-48%">Calls</text></svg>
    <svg x="10%" y="67%" width="100" height="100" class="downarrow">
        <line x1="50%" y1="0%" x2="50%" y2="80%" />
        <text x="40%" y="-48%">Instructions</text></svg></g>
</svg>
</div>
@endhtmlonly

### Background ###
#### Block Floating-Point Vectors ####

A standard (IEEE) floating-point object can exist either as a scalar, e.g.

@code{.c}
    //Single IEEE floating-point variable
    float foo;
@endcode

or as a vector, e.g.

@code{.c}
    //Array of IEEE floating-point variables
    float foo[20];
@endcode

Standard floating-point values carry both a mantissa @math{m} and an exponent @math{p}, such that the logical value 
represented by such a variable is @math{m\cdot2^p}. When you have a vector of standard floating-point values, each
element of the vector carries its own mantissa and its own exponent: @math{m[k]\cdot2^{p[k]}}


@htmlonly
<div style="text-align:center">
<svg width="510" height="200">
    <defs>    
        <svg width="60" height="150" class="float_elm-bounds" id="float-fields">
            <rect width="100%" height="100%" class="float_elm-outer"/>
            <rect width="50" height="20" x="5" y="5" class="float_elm-sign" />
            <text y="-4" class="float_elm-sign">
                <tspan x="30" dy="10">sign</tspan>
                <tspan x="30" dy="11" style="font-size:80%;">(1 bit)</tspan>
            </text>
            <rect width="50" height="40" x="5" y="25" class="float_elm-exponent" />
            <text y="25" class="float_elm-sign">
                <tspan x="30" dy="10">exponent</tspan>
                <tspan x="30" dy="11" style="font-size:80%;">(8 bits)</tspan>
            </text>
            <rect width="50" height="60" x="5" y="65" class="float_elm-mantissa" />
            <text y="65" class="float_elm-sign">
                <tspan x="30" dy="10">mantissa</tspan>
                <tspan x="30" dy="11" style="font-size:80%;">(23 bits)</tspan>
            </text>
        </svg>
    </defs>
    <style>
    svg.float_elm-bounds > rect.float_elm-outer { fill:#149; stroke:black; rx:5;ry:5; }
    svg.float_elm-bounds > rect.float_elm-sign { fill:#AFA; stroke:#006; rx:5;ry:5; }
    svg.float_elm-bounds > rect.float_elm-exponent { fill:#FAA; stroke:#006; rx:5;ry:5; }
    svg.float_elm-bounds > rect.float_elm-mantissa { fill:#AAF; stroke:#006; rx:5;ry:5; }
    svg.float_elm-bounds > text { fill:black; dominant-baseline:hanging; text-anchor:middle; font-size:60%; font-family:monospace}
    svg.float_elm > text.index {fill:white; dominant-baseline:hanging; text-anchor:middle; font-size:150%}
    </style>
    <rect fill="#CCC" rx="5" ry="5" height="100%" width="100%" />
    <svg width="60" height="150" class="float_elm" x="5" y="5">
        <use xlink:href="#float-fields" />
        <text x="30" y="125" class="index">0</text> </svg>
    <svg width="60" height="150" class="float_elm" x="65" y="5">
        <use xlink:href="#float-fields" />
        <text x="30" y="125" class="index">1</text> </svg>
    <svg width="60" height="150" class="float_elm" x="125" y="5">
        <use xlink:href="#float-fields" />
        <text x="30" y="125" class="index">2</text> </svg>
    <svg width="60" height="150" class="float_elm" x="185" y="5">
        <use xlink:href="#float-fields" />
        <text x="30" y="125" class="index">3</text> </svg>
    <text x="285" y="75" style="font-size:7em" >...</text>
    <svg width="60" height="150" class="float_elm" x="385" y="5">
        <use xlink:href="#float-fields" />
        <text x="30" y="125" class="index">18</text> </svg>
    <svg width="60" height="150" class="float_elm" x="445" y="5">
        <use xlink:href="#float-fields" />
        <text x="30" y="125" class="index">19</text> </svg>
    <text x="50%" y="185" style="font-family:monospace; font-size:2em; text-anchor:middle;">float foo[20];</text>
</svg>
</div>
@endhtmlonly


By contrast, block floating-point objects have a vector of mantissas @math{\bar{m}} which all share the same exponent
@math{p}, such that the logical value of the element at index @math{k} is @math{m[k]\cdot2^p}.

@code{.c}
    struct {
        // Array of mantissas
        int32_t mant[20];
        // Shared exponent
        int32_t exp;
    } bfp_vect;
@endcode

@htmlonly
<div style="text-align:center">
<svg width="900" height="200">
    <defs>    
        <svg width="60" height="150" class="mant_elm-bounds" id="mant-fields">
            <rect width="100%" height="100%" class="mant_elm-outer"/>
            <rect width="50" height="115" x="5" y="5" class="mant_elm-mantissa" />
            <text y="5" class="mant_elm-sign">
                <tspan x="30" dy="10">mantissa</tspan>
                <tspan x="30" dy="11" style="font-size:80%;">(32 bits)</tspan>
            </text>
        </svg>
    </defs>
    <style>
    svg.mant_elm-bounds > rect.mant_elm-outer { fill:#149; stroke:black; rx:5;ry:5; }
    svg.mant_elm-bounds > rect.mant_elm-mantissa { fill:#AAF; stroke:#006; rx:5;ry:5; }
    svg.mant_elm-bounds > text { fill:black; dominant-baseline:hanging; text-anchor:middle; font-size:60%; font-family:monospace}
    svg.mant_elm > text.index {fill:white; dominant-baseline:hanging; text-anchor:middle; font-size:150%}
    </style>
    <rect fill="#CCC" rx="5" ry="5" height="100%" width="510" />
    <svg width="60" height="150" class="mant_elm" x="5" y="5">
        <use xlink:href="#mant-fields" />
        <text x="30" y="125" class="index">0</text> </svg>
    <svg width="60" height="150" class="mant_elm" x="65" y="5">
        <use xlink:href="#mant-fields" />
        <text x="30" y="125" class="index">1</text> </svg>
    <svg width="60" height="150" class="mant_elm" x="125" y="5">
        <use xlink:href="#mant-fields" />
        <text x="30" y="125" class="index">2</text> </svg>
    <svg width="60" height="150" class="mant_elm" x="185" y="5">
        <use xlink:href="#mant-fields" />
        <text x="30" y="125" class="index">3</text> </svg>
    <text x="285" y="75" style="font-size:7em" >...</text>
    <svg width="60" height="150" class="mant_elm" x="385" y="5">
        <use xlink:href="#mant-fields" />
        <text x="30" y="125" class="index">18</text> </svg>
    <svg width="60" height="150" class="mant_elm" x="445" y="5">
        <use xlink:href="#mant-fields" />
        <text x="30" y="125" class="index">19</text> </svg>
    <text x="270" y="185" style="font-family:monospace; font-size:2em; text-anchor:middle;">bfp_vect.mant[]</text>
    <!-- -->
    <rect fill="#CCC" rx="5" ry="5" x="620" height="100%" width="220" />
    <svg x="700" y="5" width="60" height="150" class="float_elm-bounds" id="mant-fields">
        <rect width="100%" height="100%" class="float_elm-outer"/>
        <rect width="50" height="115" x="5" y="5" class="float_elm-exponent" />
        <text y="5" class="float_elm-sign">
            <tspan x="30" dy="10">exponent</tspan>
            <tspan x="30" dy="11" style="font-size:80%;">(32 bits)</tspan>
        </text>
    </svg>
    <text x="730" y="185" style="font-family:monospace; font-size:2em; text-anchor:middle;">bfp_vect.exp</text>
</svg>
</div>
@endhtmlonly

#### Headroom ####

With a given exponent, @math{p}, the largest value that can be represented by a 32-bit BFP vector is given by
a maximal mantissa (@math{2^{31}-1}), for a logical value of @math{(2^{31}-1)\cdot2^p}. The smallest non-zero value
that an element can represent is @math{1\cdot2^p}.

Because all elements must share a single exponent, in order to avoid overflow or saturation of the largest magnitude
values, the exponent of a BFP vector is constrained by the element with the largest (logical) value. The drawback to
this is that when the elements of a BFP vector represent a large dynamic range -- that is, where the largest magnitude
element is many, many times larger than the smallest (non-zero) magnitude element -- the smaller magnitude elements
effectively have fewer bits of precision.

Consider a 2-element BFP vector intended to carry the values @math{2^{20}} and @math{255 \cdot 2^{-10}}. One way this
vector can be represented is to use an exponent of @math{0}.

@code{.c}
    struct {
        int32_t mant[2];
        int32_t exp;
    } vect = { { (1<<20), (0xFF >> 10) }, 0 };
@endcode

@htmlonly
<div style="text-align:center">
<svg width="900" height="170">
    <style>
        rect.bit-block { stroke:black; }
        rect.whole-bits { fill:#AAF; }
        rect.frac-bits { fill:#CCF; }
        text.whole-bits { dominant-baseline:hanging; text-anchor:end; font-family:monospace;  }
        text.frac-bits { dominant-baseline:hanging; text-anchor:start; font-family:monospace; fill:red;  }
        text.var-label { dominant-baseline:hanging; text-anchor:end; font-family:monospace;  }
    </style>
    <defs>
    </defs>
    <!-- -->
    <rect fill="#CCC" rx="5" ry="5" height="100%" width="100%" />
    <rect x="120" y="45" width="560" height="55" class="bit-block whole-bits" />
    <rect x="680" y="45" width="200" height="55" class="bit-block frac-bits" />
    <svg x="3" y="25" width="100%" height="20">
        <text x="90" y="0" class="var-label">exponent</text>
        <text x="140" dx="-8" style="dominant-baseline:hanging; text-anchor:start; font-family:monospace; ">0</text>
    </svg>
    <svg x="3" y="50" width="100%" height="20">
        <text x="90" y="0" class="var-label">mant[0]</text>
        <text x="680" dx="-8" letter-spacing="0" class="whole-bits">
            <tspan fill="green">0 0 0 0 0 0 0 0 0 0</tspan>
            <tspan>0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0</tspan></text>
        <text x="680" dx="3" class="frac-bits"> 0 0 0 0 0 0 0 0 0 0 0</text>
    </svg>
    <svg x="3" y="75" width="100%" height="20">
        <text x="90" y="0" class="var-label">mant[1]</text>
        <text x="680" dx="-8" letter-spacing="0" class="whole-bits">
            <tspan fill="green">0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0</tspan>
            <tspan></tspan></text>
        <text x="680" dx="3" class="frac-bits"> 0 0 1 1 1 1 1 1 1 1 0 </text>
    </svg>
    <text x="400" y="130" style="font-size:2em; text-anchor:middle; ">Integer Bits</text>
    <text x="400" y="155" style="font-size:1.5em; text-anchor:middle; ">(Kept)</text>
    <text x="780" y="130" style="font-size:2em; text-anchor:middle; ">Fractional Bits</text>
    <text x="780" y="155" style="font-size:1.5em; text-anchor:middle; ">(Discarded)</text>
</svg>
</div>
@endhtmlonly

In the diagram above, the fractional bits (shown in red text) are discarded, as the mantissa is only 32 bits. Then, with 
@math{0} as the exponent, `mant[1]` underflows to @math{0}. Meanwhile, the 12 most significant bits of `mant[0]`
are all zeros.

Headroom is the number of bits that a mantissa can be left-shifted without losing any information. In the the diagram,
the bits corresponding to headroom are shown in green text. Here `mant[0]` has 10 bits of headroom and `mant[1]` has
a full 32 bits of headroom. (`mant[0]` does not have 11 bits of headroom because in two's complement the MSb serves as
a sign bit). The headroom for a BFP vector is the _minimum_ of headroom amongst each of its elements; in this case, 10 
bits.

If we remove headroom from one mantissa of a BFP vector, all other mantissas must shift by the same number of bits, and
the vector's exponent must be adjusted accordingly. A left-shift of one bit corresponds to reducing the exponent by 1.

In this case, if we remove 10 bits of headroom and subtract 10 from the exponent we get the following:

@htmlonly
<div style="text-align:center">
<svg width="900" height="170">
    <style>
        rect.bit-block { stroke:black; }
        rect.whole-bits { fill:#AAF; }
        rect.frac-bits { fill:#CCF; }
        text.whole-bits { dominant-baseline:hanging; text-anchor:end; font-family:monospace;  }
        text.frac-bits { dominant-baseline:hanging; text-anchor:start; font-family:monospace; fill:red;  }
        text.var-label { dominant-baseline:hanging; text-anchor:end; font-family:monospace;  }
    </style>
    <defs>
    </defs>
    <!-- -->
    <rect fill="#CCC" rx="5" ry="5" height="100%" width="100%" />
    <rect x="120" y="45" width="560" height="55" class="bit-block whole-bits" />
    <rect x="680" y="45" width="200" height="55" class="bit-block frac-bits" />
    <svg x="3" y="25" width="100%" height="20">
        <text x="90" y="0" class="var-label">exponent</text>
        <text x="140" dx="-8" style="dominant-baseline:hanging; text-anchor:start; font-family:monospace; ">-10</text>
    </svg>
    <svg x="3" y="50" width="100%" height="20">
        <text x="90" y="0" class="var-label">mant[0]</text>
        <text x="680" dx="-8" letter-spacing="0" class="whole-bits">
            <tspan fill="green"></tspan>
            <tspan>0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0</tspan></text>
        <text x="680" dx="3" class="frac-bits"> 0 0 0 0 0 0 0 0 0 0 0</text>
    </svg>
    <svg x="3" y="75" width="100%" height="20">
        <text x="90" y="0" class="var-label">mant[1]</text>
        <text x="680" dx="-8" letter-spacing="0" class="whole-bits">
            <tspan fill="green">0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0</tspan>
            <tspan>0 1 1 1 1 1 1 1 1</tspan></text>
        <text x="680" dx="3" class="frac-bits"> 0 0 0 0 0 0 0 0 0 0 0 </text>
    </svg>
    <text x="400" y="130" style="font-size:2em; text-anchor:middle; ">Integer Bits</text>
    <text x="400" y="155" style="font-size:1.5em; text-anchor:middle; ">(Kept)</text>
    <text x="780" y="130" style="font-size:2em; text-anchor:middle; ">Fractional Bits</text>
    <text x="780" y="155" style="font-size:1.5em; text-anchor:middle; ">(Discarded)</text>
</svg>
</div>
@endhtmlonly

Now, no information is lost in either element. One of the main goals of BFP arithmetic is to keep the headroom in BFP
vectors to the minimum necessary (equivalently, keeping the exponent as small as possible). That allows for maximum 
effective precision of the elements in the vector.

Note that the headroom of a vector also tells you something about the size of the largest magnitude mantissa in the
vector. That information (in conjunction with exponents) can be used to determine the largest possible output of an 
operation without having to look at the mantissas.

For this reason, the BFP vectors in `lib_xs3_math` carry a field which tracks their current headroom. The BFP functions
in the high-level API use this property to make determinations about how best to preserve precision.

### High-level API ###

In the high-level API the BFP vectors area buffer represented by C structures such as `bfp_s16_t`, `bfp_s32_t`, or 
`bfp_complex_s32_t`. These objects contain a pointer to the dat carrying the content of the vector, as well 
as information about the length, headroom and exponent of the BFP vector.

Below is the definition of `bfp_s32_t` from xs3_math_types.h.

@snippet{lineno} api/xs3_math_types.h bfp_s32_t 

The functions in @ref bfp_vector_functions32 take `bfp_s32_t` references as input and output parameters.

Functions in the high-level API generally are prefixed with `bfp_`.

#### Initializing BFP Vectors ####

Before calling these functions, the BFP vectors represented by the arguments must be initialized. For `bfp_s32_t` this 
is accomplished with bfp_s32_init().

@code{.c}
    #define LEN (20)

    //The object representing the BFP vector
    bfp_s32_t bfp_vect; 

    // buffer backing bfp_vect
    int32_t data_buffer[LEN]; 
    for(int i = 0; i < LEN; i++) data_buffer[i] = i;

    // The initial exponent associated with bfp_vect
    exponent_t initial_exponent = 0; 

    // If non-zero, bfp_s32_init() will compute the headroom currently present in data_buffer. 
    // Otherwise, headroom is initialized to 0 (which is always safe but may not be optimal)
    unsigned calculate_headroom = 1; 

    // Initialize the vector object
    bfp_s32_init(&bfp_vec, data_buffer, initial_exponent, LEN, calculate_headroom);

    // Go do stuff with bfp_vect
    ...
@endcode

Once initialized, the exponent and mantissas of the vector can be accessed by `bfp_vect->exp` and `bfp_vect->data[]`
respectively, with the logical (floating-point) value of element `k` being given by 
`ldexp(bfp_vect->data[k], bfp_vect->exp)`.

#### BFP Arithmetic Functions ####

The following snippet shows a function `foo()` which takes 3 BFP vectors, `a`, `b` and `c`, as arguments. It multiplies
together `a` and `b` element-wise, and then subtracts `c` from the product. In this example both operations are
performed in-place on `a`. (See bfp_s32_mul() and bfp_s32_sub() for more information about those functions)

@code{.c}

    void foo(bfp_s32_t* a, const bfp_s32_t* b, const bfp_s32_t* c)
    {
        // Multiply together a and b, updating a with the result.
        bfp_s32_mul(a, a, b);

        // Subtract c from the product, again updating a with the result.
        bfp_s32_sub(a, a, c);
    }

@endcode

The caller of `foo()` can then access the results through `a`. Note that the pointer `a->data` was not modified during
this call.

### Low-level API ###

The functions in the low-level API are optimized for performance. They do very little to protect the user from mangling
their data by arithmetic saturation/overflows or underflows. Functions in the low-level API are generally prefixed with
`xs3_`.

As an example of a function from the low-level API, see xs3_vect_s32_mul() from xs3_bfp_low.h, which multiplies together
two `int32_t` vectors element by element.

@snippet{lineno} api/vect/xs3_vect_s32.h xs3_vect_s32_mul

This function takes two `int32_t` arrays, `b` and `c`, as inputs and one `int32_t` array, `a`, as output. `length` 
indicates the number of elements in each array. The final two parameters, `b_shr` and `c_shr`, are the arithmetic 
right-shifts applied to each element of `b` and `c` before they are multiplied together.

Why the right-shifts? This reflects details of the XS3 instructions which target the VPU. With the XS3 VPU, 
multiplications of 32-bit numbers always include a compulsory (rounding) right-shift by 30 bits. So, to multiply two
vectors element-wise with managed precision, the inputs must be shifted before multiplication to ensure the results
are scaled as desired.

Contrast this with xs3_vect_s16_mul():

@snippet{lineno} api/vect/xs3_vect_s16.h xs3_vect_s16_mul

The parameters are similar here, but instead of `b_shr` and `c_shr`, there's only an `a_shr`. This reflects the fact 
that products of 16-bit numbers can be accumulated without a compulsory right-shift, and so there is no risk of losing
information by multiplying. Instead, a single right-shift can be applied to the 32-bit product to correctly scale the
result.

Both xs3_vect_s32_mul() and xs3_vect_s16_mul() return the headroom of the output vector `a`.

Functions in the low-level API are in many cases closely tied to the instruction set architecture for XS3. As such, when
more efficient algorithms are found to perform an operation these functions are more likely to change.