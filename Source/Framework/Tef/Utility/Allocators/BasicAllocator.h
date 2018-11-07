#pragma once

#include "Prerequisites/Types.h"
#include "Prerequisites/StdHeaders.h"

#include <new>
#include <memory>

namespace te
{
    /* ###################################################################
    *  ############# BASIC ALLOCATOR #####################################
    *  ################################################################ */

    class TE_UTILITY_EXPORT BasicAllocator
    {
    public:
        BasicAllocator() {}
        ~BasicAllocator() {}

        void* Allocate(size_t amount)
        {
            return malloc(amount);
        }

        void Deallocate(void* data)
        {
            free(data);
        }

    protected:
        BasicAllocator(BasicAllocator const&) = delete;
        BasicAllocator& operator=(BasicAllocator const&) = delete;
    };

    /* ###################################################################
    *  ############# ALLOCATOR METHODS ###################################
    *  ################################################################ */

    TE_UTILITY_EXPORT BasicAllocator& gBasicAllocator();

    TE_UTILITY_EXPORT void* te_basic_allocate(UINT32 numBytes);
    TE_UTILITY_EXPORT void te_basic_deallocate(void* data);

    /* ###################################################################
    *  ############# MEMORY ALLOCATOR FOR STD ALLOCATOR ##################
    *  ################################################################ */

    /**
    * Memory allocator using BasicAllocator
    */

    template<>
    class TE_UTILITY_EXPORT MemoryAllocator<BasicAllocator> : public MemoryAllocatorBase
    {
    public:
        static void* Allocate(size_t bytes)
        {
#if TE_DEBUG_MODE == 1
            AddNewCount();
#endif
            return gBasicAllocator().Allocate((UINT32)bytes);
        }

        static void Deallocate(void* ptr)
        {
#if TE_DEBUG_MODE == 1
            AddFreeCount();
#endif
            gBasicAllocator().Deallocate(ptr);
        }
    };
}
