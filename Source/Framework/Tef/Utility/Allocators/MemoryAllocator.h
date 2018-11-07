#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Prerequisites/Types.h"

#include <new>

#pragma once
#undef min
#undef max

#if TE_PLATFORM == TE_PLATFORM_LINUX
#  include <malloc.h>
#endif

namespace te {

    /* ###################################################################
    *  ############# MEMORY COUNTER ######################################
    *  ################################################################ */

    /**
    * Thread safe class used for storing total number of memory allocations and deallocations, primarily for statistic
    * purposes.
    */
    class TE_UTILITY_EXPORT MemoryCounter
    {
    public:
        static UINT64 GetNumAllocs()
        {
            return News;
        }

        static UINT64 GetNumFrees()
        {
            return Frees;
        }

    protected:
        friend class MemoryAllocatorBase;

        static void AddNewCount() { News++; }
        static void AddFreeCount() { Frees++; }

        static UINT64 News;
        static UINT64 Frees;
    };

    /**
    * Base class all memory allocators need to inherit. Provides allocation and free counting.
    */
    class TE_UTILITY_EXPORT MemoryAllocatorBase
    {
    protected:
        static void AddNewCount() { MemoryCounter::AddNewCount(); }
        static void AddFreeCount() { MemoryCounter::AddFreeCount(); }
    };

    /* ###################################################################
    *  ############# MEMORY ALLOCATOR BASE ###############################
    *  ################################################################ */

    /**
    * Memory allocator providing a generic implementation. Specialize for specific categories as needed.
    */
    template<class T>
    class TE_UTILITY_EXPORT MemoryAllocator : public MemoryAllocatorBase
    {
    public:
        static void* Allocate(size_t bytes)
        {
#if TE_DEBUG_MODE == 1
            AddNewCount();
#endif
            return ::malloc(bytes);
        }

        static void Deallocate(void* ptr)
        {
#if TE_DEBUG_MODE == 1
            AddFreeCount();
#endif
            ::free(ptr);
        }
    };

    /**
    * Default allocator when you want to use default os memory management
    */
    class TE_UTILITY_EXPORT GeneralAllocator
    {
    };

    /* ###################################################################
    *  ############# ENGINE MEMORY ALLOCATION ############################
    *  ################################################################ */

    /**
    * Allocates the specified number of bytes (custom allocator)
    */
    template<class Allocator = GeneralAllocator>
    TE_UTILITY_EXPORT inline void* te_allocate(UINT32 count)
    {
        return MemoryAllocator<Allocator>::Allocate(count);
    }

    /**
    * Allocates enough bytes to hold the specified type, but doesn't construct it (custom allocator).
    */
    template<class T, class Allocator = GeneralAllocator>
    TE_UTILITY_EXPORT inline T* te_allocate()
    {
        return (T*)MemoryAllocator<Allocator>::Allocate(sizeof(T));
    }

    /**
    * Create a new object with the specified allocator and the specified parameters (custom allocator).
    */
    template<class Type, class Allocator, class... Args>
    TE_UTILITY_EXPORT inline Type* te_new(Args &&...args)
    {
        return new (te_allocate<Allocator>(sizeof(Type))) Type(std::forward<Args>(args)...);
    }

    /** Create a new object with the specified allocator and the specified parameters. */
    template<class Type, class... Args>
    TE_UTILITY_EXPORT inline Type* te_new(Args &&...args)
    {
        return new (te_allocate<GeneralAllocator>(sizeof(Type))) Type(std::forward<Args>(args)...);
    }

    /**
    * Frees all the bytes allocated at the specified location.
    */
    template<class Allocator = GeneralAllocator>
    TE_UTILITY_EXPORT inline void te_deallocate(void* ptr)
    {
        MemoryAllocator<Allocator>::Deallocate(ptr);
    }

    /** Destructs and frees the specified object. */
    template<class T, class Allocator = GeneralAllocator>
    TE_UTILITY_EXPORT inline void te_delete(T* ptr)
    {
        (ptr)->~T();
        MemoryAllocator<Allocator>::Deallocate(ptr);
    }

    /* ###################################################################
    *  ############# STL ALLOCATOR WRAPPER ###############################
    *  ################################################################ */

    /** Allocator used for the standard library */
    template <class T, class Allocator = GeneralAllocator>
    class TE_UTILITY_EXPORT StdAllocator
    {
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        StdAllocator() noexcept
        {}

        StdAllocator(Allocator* allocator) noexcept
            : _allocator(allocator)
        {}

        template<class U, class Allocator2> StdAllocator(const StdAllocator<U, Allocator2>& alloc) noexcept
            : _allocator(alloc._allocator)
        {}

        template<class U, class Allocator2> bool operator==(const StdAllocator<U, Allocator2>&) const noexcept { return true; }
        template<class U, class Allocator2> bool operator!=(const StdAllocator<U, Allocator2>&) const noexcept { return false; }
        template<class U> class rebind { public: typedef StdAllocator<U, Allocator> other; };

        /** Allocate but don't initialize number elements of type T. */
        T* allocate(const size_t num) const
        {
            if (num == 0)
            {
                return nullptr;
            }

            if (num > static_cast<size_t>(-1) / sizeof(T))
            {
                return nullptr;
            }

            void* pv = nullptr;

            if (_allocator == nullptr)
            {
                pv = te_allocate<Allocator>((UINT32)(num * sizeof(T)));
            }
            else
            {
                pv = _allocator->Allocate((num * sizeof(T)));
            }

            if (!pv)
            {
                return nullptr; // Error
            }

            return static_cast<T*>(pv);
        }

        /** Deallocate storage p of deleted elements. */
        void deallocate(T* p, size_t num) const noexcept
        {
            if (_allocator == nullptr)
            {
                te_deallocate<Allocator>((void*)p);
            }
            else
            {
                _allocator->Deallocate(p);
            }
        }

        size_t max_size() const { return std::numeric_limits<size_type>::max() / sizeof(T); }
        void construct(pointer p, const_reference t) { new (p) T(t); }
        void destroy(pointer p) { p->~T(); }

        /* This version of construct() (with a varying number of parameters)
        * seems necessary in order to use some STL data structures from
        * libstdc++-4.8, but compilation fails on OS X, hence the #if. */
        template<class U, class... Args>
        void construct(U* p, Args&&... args) { new(p) U(std::forward<Args>(args)...); }

    public:
        Allocator * _allocator;
    };
}

#include "BasicAllocator.h"
#include "LinearAllocator.h"
#include "StackAllocator.h"
#include "PoolAllocator.h"