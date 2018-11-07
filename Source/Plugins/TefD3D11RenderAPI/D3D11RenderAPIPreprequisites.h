#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#if TE_PLATFORM == TE_PLATFORM_WIN32
#   if TE_RENDER_API == TE_RENDER_API_DIRECTX11
#       include <dxgi.h>
#       include <d3d11_1.h>

#       include <DirectXMath.h>
#       include <DirectXColors.h>
#       include <DirectXPackedVector.h>

#       include <Windowsx.h>
#       include <Xinput.h>

#       if defined(TE_WIN_SDK_7)
#           include <d3dx11.h>
#           include <d3dcompiler.h>
#           include <comdecl.h>
#           include <xaudio2.h>
#           include <xaudio2fx.h>
#           include <xapofx.h>
#           include <x3daudio.h>
#           pragma warning(pop)
#           pragma comment(lib,"d3dx11.lib")
#           pragma comment(lib,"x3daudio.lib")
#           pragma comment(lib,"xapofx.lib")
#       elif defined(EGE_WIN_SDK_8) || defined(EGE_WIN_SDK_10)
#           include <d3dcompiler.h>
#           include <xaudio2.h>
#           include <xaudio2fx.h>
#           include <x3daudio.h>
#           include <xapofx.h>
#           pragma comment(lib,"xaudio2.lib")
#       endif

#       pragma comment(lib, "dxgi.lib")
#       pragma comment(lib, "d3d11.lib")
#       pragma comment(lib, "d3dcompiler.lib")
#       pragma comment(lib, "winmm.lib")
#       pragma comment(lib, "xinput9_1_0.lib")
#       pragma comment(lib, "xinput.lib")

#   else
#   endif
#else

#endif

namespace te
{
    template<typename T>
    inline void SafeReleaseCom(T& ptr)
    {
        if (ptr != nullptr)
        {
#if TE_PLATFORM == TE_PLATFORM_WIN32
            ptr->Release();
#else
            delete ptr;
#endif
            ptr = nullptr;
        }
    }

}
