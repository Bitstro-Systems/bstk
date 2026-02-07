// tinywin.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_CORE_IMPL_TINYWIN_HPP_
#define _BSTK_CORE_IMPL_TINYWIN_HPP_

#ifdef _BSTK_WINDOWS
#define NOSERVICE
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // _BSTK_WINDOWS
#endif // _BSTK_CORE_IMPL_TINYWIN_HPP_