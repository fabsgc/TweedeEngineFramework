#pragma once

#include "Error/Error.h"
#include "Debug/Debug.h"
#include "Prerequisites/Types.h"
#include "Prerequisites/StdHeaders.h"

namespace te
{
    /* ###################################################################
    *  ############# LINEAR ALLOCATOR ####################################
    *  ################################################################ */

    class TE_UTILITY_EXPORT LinearAllocator
    {
    public:
        LinearAllocator(size_t size = sizeof(UINT32) * 8192)
            : _totalSize(size)
        {
            _startPtr = malloc(_totalSize);
            Reset();
        }

        ~LinearAllocator()
        {
            free(_startPtr);
            Reset();
        }

        void * Allocate(size_t amount)
        {
            const size_t dataAddress = (size_t)_startPtr + _offset;
            size_t size = amount;
            _offset += size;

            TE_ASSERT_ERROR((_offset + size <= _totalSize), "Not enough memory allocated in stack allocator");

            return (void*)dataAddress;
        }

        void Deallocate(void* data)
        {
            return;
        }

        void Reset()
        {
            _offset = 0;
        }

    protected:
        LinearAllocator(LinearAllocator const&) = delete;
        LinearAllocator& operator=(LinearAllocator const&) = delete;

    protected:
        size_t _offset;
        size_t _totalSize;
        void * _startPtr;
    };

    /* ###################################################################
    *  ############# ALLOCATOR METHOD ####################################
    *  ################################################################ */

    LinearAllocator& gLinearAllocator();

    TE_UTILITY_EXPORT void* te_linear_allocate(UINT32 numBytes);
    TE_UTILITY_EXPORT void te_linear_deallocate(void* data);

    /* ###################################################################
    *  ############# MEMORY ALLOCATOR FOR STD ALLOCATOR ##################
    *  ################################################################ */

    /**
    * Memory allocator using LinearAllocator
    */
    template<>
    class TE_UTILITY_EXPORT MemoryAllocator<LinearAllocator> : public MemoryAllocatorBase
    {
    public:
        static void* Allocate(size_t bytes)
        {
#if TE_DEBUG_MODE == 1
            AddNewCount();
#endif
            return gLinearAllocator().Allocate((UINT32)bytes);
        }

        static void Deallocate(void* ptr)
        {
#if TE_DEBUG_MODE == 1
            AddFreeCount();
#endif
            gLinearAllocator().Deallocate(ptr);
        }
    };
}
