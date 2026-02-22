// allocator.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/allocator.hpp>

namespace bs {
    bool operator==(const allocator& _Left, const allocator& _Right) noexcept {
        return _Left.is_equal(_Right);
    }
} // namespace bs