#pragma once

#include "Prerequisites/Types.h"
#include "Error/Error.h"
#include "Debug/Debug.h"
#include "String/String.h"
#include "Prerequisites/StdHeaders.h"

namespace te
{
    /* ###################################################################
    *  ############# STACK ALLOCATOR #####################################
    *  ################################################################ */

    class TE_UTILITY_EXPORT StackAllocator
    {
    private:
        struct StackAllocationHeader {
            size_t padding;
            size_t size;
        };

    public:
        StackAllocator(size_t size = sizeof(UINT32) * 8192)
            : _totalSize(size)
        {
            _startPtr = malloc(_totalSize + _totalSize * sizeof(StackAllocationHeader));
            Reset();
        }

        ~StackAllocator()
        {
            if (_offset != 0)
            {
                TE_DEBUG("Memory from stack allocator has not been properly free");
            }
            else
            {
                free(_startPtr);
                _startPtr = nullptr;
            }

            Reset();
        }

        void * Allocate(size_t amount)
        {
            const size_t currentAddress = (size_t)_startPtr + _offset;

            size_t padding = 0;
            size_t size = amount;

            TE_ASSERT_ERROR((_offset + padding + size <= _totalSize), "Not enough memory allocated in stack allocator");

            const size_t headerAddress = currentAddress + padding;
            const size_t dataAddress = headerAddress + sizeof(StackAllocationHeader);

            StackAllocationHeader allocationHeader{ padding, size };
            StackAllocationHeader * headerPtr = (StackAllocationHeader*)headerAddress;
            headerPtr->padding = padding;
            headerPtr->size = size;

            _offset += padding + sizeof(StackAllocationHeader) + size;

            _used = _offset;
            _peak = std::max(_peak, _used);

            return (void*)dataAddress;
        }

        void Deallocate(void* data)
        {
            const size_t currentAddress = (size_t)data;
            const size_t headerAddress = currentAddress - sizeof(StackAllocationHeader);
            const StackAllocationHeader * allocationHeader{ (StackAllocationHeader *)headerAddress };

            if (currentAddress == _offset + (size_t)_startPtr - allocationHeader->size)
            {
                _offset = _offset - allocationHeader->size - sizeof(StackAllocationHeader) - allocationHeader->padding;
                _used = _offset;
            }
            else
            {
                TE_DEBUG("Memory leak");
            }
        }

        void Reset()
        {
            _offset = 0;
            _used = 0;
            _peak = 0;
        }

    protected:
        StackAllocator(StackAllocator const&) = delete;
        StackAllocator& operator=(StackAllocator const&) = delete;

    protected:
        size_t _offset;
        size_t _totalSize;
        size_t _used;
        size_t _peak;
        void * _startPtr;
    };

    /* ###################################################################
    *  ############# ALLOCATOR METHODS ###################################
    *  ################################################################ */

    TE_UTILITY_EXPORT StackAllocator& gStackAllocator();

    TE_UTILITY_EXPORT void* te_stack_allocate(UINT32 numBytes);
    TE_UTILITY_EXPORT void te_stack_deallocate(void* data);

    /* ###################################################################
    *  ############# MEMORY ALLOCATOR FOR STD ALLOCATOR ##################
    *  ################################################################ */

    /**
    * Memory allocator using StackAllocator
    */
    template<>
    class TE_UTILITY_EXPORT MemoryAllocator<StackAllocator> : public MemoryAllocatorBase
    {
    public:
        static void* Allocate(size_t bytes)
        {
#if TE_DEBUG_MODE == 1
            AddNewCount();
#endif
            return gStackAllocator().Allocate((UINT32)bytes);
        }

        static void Deallocate(void* ptr)
        {
#if TE_DEBUG_DEBUG == 1
            AddFreeCount();
#endif
            gStackAllocator().Deallocate(ptr);
        }
    };
}