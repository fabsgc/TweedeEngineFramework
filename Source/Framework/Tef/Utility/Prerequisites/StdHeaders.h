#pragma once

#ifdef __BORLANDC__
#   define __STD_ALGORITHM
#endif

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <atomic>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <chrono>
#include <ctime>
#include <memory>
#include <string>
#include <cstdio>
#include <new>

// STL containers
#include <vector>
#include <stack>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>
#include <array>

#include <unordered_map>
#include <unordered_set>

// STL algorithms & functions
#include <algorithm>
#include <functional>
#include <limits>
#include <random>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

extern "C" {
#   include <sys/types.h>
#   include <sys/stat.h>
}

#if TE_PLATFORM == TE_PLATFORM_WIN32
#  undef min
#  undef max
#	if !defined(NOMINMAX) && defined(_MSC_VER)
#		define NOMINMAX // required to stop windows.h messing up std::min
#	endif
#  if defined( __MINGW32__ )
#    include <unistd.h>
#  endif
#endif

#if TE_PLATFORM == TE_PLATFORM_LINUX
extern "C" {
#   include <unistd.h>
#   include <dlfcn.h>
}
#endif

#if TE_PLATFORM == TE_PLATFORM_OSX
extern "C" {
#   include <unistd.h>
#   include <sys/param.h>
#   include <CoreFoundation/CoreFoundation.h>
}
#endif

#include "Allocators/BasicAllocator.h"

#if TE_PLATFORM == TE_PLATFORM_WIN32
#   include <Windows.h>
#   include <fcntl.h>
#   include <io.h>

#   ifndef UNICODE
#       define UNICODE
#   endif

#   ifndef _UNICODE
#       define _UNICODE
#   endif
#endif

#if TE_PLATFORM == TE_PLATFORM_WIN32
#   define TE_SLEEP(ms) Sleep(ms)
#elif TE_PLATFORM == TE_PLATFORM_LINUX
#   define TE_SLEEP(ms) usleep(ms)
#else
#   define TE_SLEEP(ms) usleep(ms)
#endif

namespace te
{
    /* ###################################################################
    *  ############# STL CONTAINER SNIPPETS ##############################
    *  ################################################################ */

    template <typename T>
    using SPtr = std::shared_ptr<T>;

    template <typename T, typename Allocator = GeneralAllocator>
    using UPtr = std::unique_ptr<T, decltype(&te_delete<T, Allocator>)>;

    template <typename T, typename A = StdAllocator<T, BasicAllocator>>
    using Vector = std::vector<T, A>;

    template <typename T, typename A = StdAllocator<T, BasicAllocator>>
    using List = std::list<T, A>;

    template <typename T, typename A = StdAllocator<T, BasicAllocator>>
    using Set = std::set<T, A>;

    template <typename L, typename R>
    using Pair = std::pair<L, R>;

    template <typename K, typename V, typename P = std::less<K>, typename A = StdAllocator<Pair<const K, V>, BasicAllocator>>
    using Map = std::map<K, V, P, A>;

    /* ###################################################################
    *  ############# SHARED PTR INSTANTIATION ############################
    *  ################################################################ */

    /**
    * \brief Create a new shared pointer using a custom allocator instance.
    */
    template<class Type, class Allocator>
    decltype(auto) te_shared_ptr_allocator_new(Allocator* allocator)
    {
        Type* ptr = (Type*)allocator->Allocate(sizeof(Type));

        auto deleter = [&](Type* ptr) {
            allocator->Deallocate(ptr);
        };

        return std::shared_ptr<Type>(ptr, deleter);
    }

    /**
    * \brief Create a new shared pointer using a custom allocator category.
    */
    template<class Type, class Allocator, class... Args>
    SPtr<Type> te_shared_ptr_new(Args &&... args)
    {
        return std::allocate_shared<Type>(StdAllocator<Type, Allocator>(), std::forward<Args>(args)...);
    }

    /**
    * \brief Create a new shared pointer using the default allocator category
    */
    template<class Type, class... Args>
    SPtr<Type> te_shared_ptr_new(Args &&... args)
    {
        return std::allocate_shared<Type>(StdAllocator<Type, BasicAllocator>(), std::forward<Args>(args)...);
    }

    /**
    * \brief Create a new shared pointer from a previously constructed object.
    *        Pointer specific data will be allocated using the provided allocator category.
    */
    template<class Type, class MainAllocator = BasicAllocator, class PtrDataAllocator = BasicAllocator>
    SPtr<Type> te_shared_ptr(Type* data, PtrDataAllocator* allocator = nullptr)
    {
        if (allocator != nullptr)
        {
            return SPtr<Type>(data, &te_delete<Type, MainAllocator>, StdAllocator<Type, PtrDataAllocator>(allocator));
        }
        else
        {
            return SPtr<Type>(data, &te_delete<Type, MainAllocator>, StdAllocator<Type, PtrDataAllocator>());
        }
    }

    /**
    * \brief Create a new unique pointer from a previously constructed object.
    *        Pointer specific data will be allocated using the provided allocator instance
    */
    template<class Type, class Allocator>
    decltype(auto) te_shared_ptr_allocator(Type* data, Allocator* allocator)
    {
        auto deleter = [&](Type* ptr) {
            allocator->Deallocate(ptr);
        };

        return std::shared_ptr<Type, decltype(deleter)>(data, deleter);
    }

    /* ###################################################################
    *  ############# UNIQUE PTR INSTANTIATION ############################
    *  ################################################################ */

    /**
    * \brief Create a new unique pointer using a custom allocator instance.
    */
    template<class Type, class Allocator>
    decltype(auto) te_unique_ptr_allocator_new(Allocator* allocator)
    {
        Type* ptr = (Type*)allocator->Allocate(sizeof(Type));

        auto deleter = [&](Type* ptr) {
            allocator->Deallocate(ptr);
        };

        return std::unique_ptr<Type, decltype(deleter)>(ptr, deleter);
    }

    /**
    * \brief Create a new unique pointer from a previously constructed object.
    *        Pointer specific data will be allocated using the provided allocator category.
    */
    template<class Type, class Allocator = GeneralAllocator>
    UPtr<Type, Allocator> te_unique_ptr(Type* data)
    {
        return std::unique_ptr<Type, decltype(&te_delete<Type, Allocator>)>(data, te_delete<Type, Allocator>);
    }

    /**
    * \brief Create a new unique pointer using a custom allocator category.
    */
    template<class Type, class Allocator = GeneralAllocator, class... Args>
    UPtr<Type> te_unique_ptr_new(Args &&... args)
    {
        Type*rawPtr = (Type*)te_allocate<Type, Allocator>(std::forward<Args>(args)...);
        return te_unique_ptr<Type, Allocator>(rawPtr);
    }

    /**
    * \brief Create a new unique pointer from a previously constructed object.
    *        Pointer specific data will be allocated using the provided allocator instance
    */
    template<class Type, class Allocator>
    decltype(auto) te_unique_ptr_allocator(Type* data, Allocator* allocator)
    {
        auto deleter = [&](Type* ptr) {
            allocator->Deallocate(ptr);
        };

        return std::unique_ptr<Type, decltype(deleter)>(data, deleter);
    }
}