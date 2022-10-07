

// This file is here to close the loop on issue #106 (https://github.com/xmos/lib_xcore_math/issues/106)
// #106 found that if "xmath/xmath.h" is included from an XC file there are compilation errors 
// because of differences between the C/C++ behavior and XC behavior.


// In the future if similar errors are made in this library, this file should have compilation
// errors and we'll know early. 
// I've confirmed that, prior to fixing #106) I do see this build error with this file.

#include "xmath/xmath.h"

unsigned dummy_val = 0;