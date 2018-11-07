#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Assimp/include/assimp/Importer.hpp"
#include "Assimp/include/assimp/Scene.h"
#include "Assimp/include/assimp/Postprocess.h"

#include "TinyXML/tinyxml2.h"

#include "Json/Json.h"

#if TE_PLATFORM == TE_PLATFORM_WIN32
#   include "DDSTextureLoader/DDSTextureLoader.h"
#endif

#if TE_DEBUG_MODE == 1
#   if !defined(HR) && TE_PLATFORM == TE_PLATFORM_WIN32
#       define HR(x, message)                           \
            {                                           \
                HRESULT hr = (x);                       \
                if(FAILED(hr))                          \
                {                                       \
                    TE_ASSERT_ERROR(false, message);    \
                }                                       \
            }
#   endif
#else
#   ifndef HR
#       define HR(x, message) (x)
#   endif
#endif 

namespace te
{
    template<typename T>
    inline void SafeRelease(T& ptr)
    {
        if (ptr != nullptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }

    class CoreApplication;
}