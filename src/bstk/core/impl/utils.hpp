// utils.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_CORE_IMPL_UTILS_HPP_
#define _BSTK_CORE_IMPL_UTILS_HPP_

// enables converting an expression into a UTF-8 string
#define _STRINGIFY_EXPR(_Expr) #_Expr
#define _STRINGIFY(_Expr)      _STRINGIFY_EXPR(_Expr)
#endif // _BSTK_CORE_IMPL_UTILS_HPP_