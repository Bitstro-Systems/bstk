// version.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_CORE_VERSION_HPP_
#define _BSTK_CORE_VERSION_HPP_
#include <bstk/core/impl/utils.hpp>

// Note: Encodes a version number from major, minor, and patch components into a 64-bit integer.
//       The major version consumes 16 bits, while the minor and patch versions each consume 24 bits.
#define _ENCODE_VERSION(_Major, _Minor, _Patch) ((_Major << 48ULL) | (_Minor << 24ULL) | _Patch)

// defines the latest BSTK library version (synchronized with the version specified in 'res/bstk.rc')
#define BSTK_VERSION        _ENCODE_VERSION(_BSTK_VERSION_MAJOR, _BSTK_VERSION_MINOR, _BSTK_VERSION_PATCH)
#define BSTK_VERSION_STRING _STRINGIFY(_BSTK_VERSION_MAJOR._BSTK_VERSION_MINOR._BSTK_VERSION_PATCH)

// checks whether the current version is greater than or equal to the specified version
#define BSTK_VERSION_SUPPORTED(_Major, _Minor, _Patch) \
    (BSTK_VERSION >= _ENCODE_VERSION(_Major, _Minor, _Patch))
#endif // _BSTK_CORE_VERSION_HPP_