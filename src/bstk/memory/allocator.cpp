// allocator.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/allocator.hpp>
#include <bstk/memory/impl/allocator.hpp>

namespace bs {
    bool operator==(const allocator& _Left, const allocator& _Right) noexcept {
        return _Left.is_equal(_Right);
    }

    allocator& get_allocator() noexcept {
        return bstk::_Get_runtime_allocator()._Get_wrapped();
    }

    void set_allocator(allocator& _Al) noexcept {
        bstk::_Get_runtime_allocator()._Set_wrapped(_Al);
    }
} // namespace bs