#pragma once

namespace te
{
    class BasicAllocator;
    class LinearAllocator;
    template<size_t ElementSize, size_t ElementsPerBlock, size_t Alignment> class PoolAllocator;
    class StackAllocator;

    class MemoryCounter;
    class MemoryAllocatorBase;
    class GeneralAllocator;

    class Console;

    class DynLib;
    class DynLibManager;

    template<class T> class Module;

    class Time;
    class Timer;

    class NonCopyable;
}