#include "Prerequisites/PrerequisitesUtil.h"

#if TE_PLATFORM == TE_PLATFORM_WIN32
#   if defined(TE_WIN_SDK_7)
#      undef _WIN32_WINNT
#      define WIN32_LEAN_AND_MEAN
#      define _WIN32_WINNT _WIN32_WINNT_WIN7
#      pragma warning(push)
#      pragma warning( disable : 4005 )
#   elif defined(TE_WIN_SDK_8)
#      define WIN32_LEAN_AND_MEAN
#      define _WIN32_WINNT _WIN32_WINNT_WIN8
#   elif defined(TE_WIN_SDK_10)
#      define WIN32_LEAN_AND_MEAN
#      define _WIN32_WINNT _WIN32_WINNT_WIN10
#   endif

#   if TE_RENDER_API == TE_RENDER_API_DIRECTX11
#       include <dxgi.h>
#       include <d3d11_1.h>

#       include <DirectXMath.h>
#       include <DirectXColors.h>
#       include <DirectXPackedVector.h>

#       include <Windowsx.h>
#       include <Xinput.h>

#       if defined(TE_WIN_SDK_7)
#           include <F:\softwares\Microsoft DirectX SDK (June 2010)\Include\d3dx11.h>
#           include <F:\softwares\Microsoft DirectX SDK (June 2010)\Include\d3dcompiler.h>
#           include <F:\softwares\Microsoft DirectX SDK (June 2010)\Include\comdecl.h>
#           include <F:\softwares\Microsoft DirectX SDK (June 2010)\Include\xaudio2.h>
#           include <F:\softwares\Microsoft DirectX SDK (June 2010)\Include\xaudio2fx.h>
#           include <F:\softwares\Microsoft DirectX SDK (June 2010)\Include\xapofx.h>
#           include <F:\softwares\Microsoft DirectX SDK (June 2010)\Include\x3daudio.h>
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
