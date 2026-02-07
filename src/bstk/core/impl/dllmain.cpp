// dllmain.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#ifdef _BSTK_WINDOWS
#include <bstk/core/impl/tinywin.hpp>

// Note: Unlike Linux shared objects (SOs), Windows dynamic-link libraries (DLLs) require
//       the definition of an entry point (DllMain), even if it does nothing.
int __stdcall DllMain(HMODULE _Module, unsigned long _Reason, void*) {
    if (_Reason == DLL_PROCESS_ATTACH) {
        ::DisableThreadLibraryCalls(_Module);
    }

    return 1;
}
#endif // _BSTK_WINDOWS