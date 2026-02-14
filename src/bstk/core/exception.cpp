// exception.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/exception.hpp>

namespace bs {
    void allocation_failure::raise() {
        throw allocation_failure{};
    }

    void allocation_limit_exceeded::raise() {
        throw allocation_limit_exceeded{};
    }

    void resource_overrun::raise() {
        throw resource_overrun{};
    }
} // namespace bs