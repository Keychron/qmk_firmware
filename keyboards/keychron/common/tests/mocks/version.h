// SPDX-License-Identifier: GPL-2.0-or-later
// Shadow of version.h for the OpenRGB unit tests.
//
// openrgb.c reads QMK_VERSION as a char array and stringizes PRODUCT /
// MANUFACTURER via STR(). Provide a 2-level stringize helper (matching QMK's
// usual STR macro) and string-literal-able product/manufacturer tokens.
#pragma once
#include "openrgb_test_shim.h"

#ifndef QMK_VERSION
#    define QMK_VERSION "test"
#endif

#ifndef PRODUCT
#    define PRODUCT Keychron K2 HE
#endif

#ifndef MANUFACTURER
#    define MANUFACTURER Keychron
#endif

#ifndef STR
#    define STR_HELPER(x) #x
#    define STR(x) STR_HELPER(x)
#endif
