// export.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_CORE_EXPORT_HPP_
#define _BSTK_CORE_EXPORT_HPP_

#ifdef _BSTK_WINDOWS
// Windows shared libraries (DLLs) require explicit export/import attributes
#ifdef _BSTK_BUILD
#define _BSTK_EXPORT __declspec(dllexport)
#else // ^^^ _BSTK_BUILD ^^^ / vvv !_BSTK_BUILD vvv
#define _BSTK_EXPORT __declspec(dllimport)
#endif // _BSTK_BUILD
#else // ^^^ _BSTK_WINDOWS ^^^ / vvv _BSTK_LINUX vvv
#define _BSTK_EXPORT
#endif // _BSTK_WINDOWS
#endif // _BSTK_CORE_EXPORT_HPP_