// utility.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_CORE_UTILITY_HPP_
#define _BSTK_CORE_UTILITY_HPP_

// enables converting an expression into a UTF-8 string
#define _BSTK_STRINGIFY_EXPR(_Expr) #_Expr
#define _BSTK_STRINGIFY(_Expr)      _BSTK_STRINGIFY_EXPR(_Expr)

// prevents compiler from generating vtable for abstract classes
#ifdef _BSTK_MSVC
#define _BSTK_NOVTABLE __declspec(novtable)
#else // ^^^ _BSTK_MSVC ^^^ / vvv _BSTK_GCC vvv 
#define _BSTK_NOVTABLE
#endif // _BSTK_MSVC
#endif // _BSTK_CORE_UTILITY_HPP_