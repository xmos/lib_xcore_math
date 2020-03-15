

Introduction                   {#mainpage}
============

\tableofcontents

`lib_xs3_math` is a library of optimized math functions for taking advantage of the vector processing unit (VPU) on the XMOS XS3 architecture.


### API ### 

The API is documented in the following header files.

#### High Level ####

#### Low Level ####

The low-level API is more powerful, but requires manual management of vector headroom. If not managed correctly, excessive information may be lost due to saturation or underflow. 

xs3_bfp_init_low.h: Low level vector initialization functions.

xs3_bfp_low.h: Low level arithmetic functions.