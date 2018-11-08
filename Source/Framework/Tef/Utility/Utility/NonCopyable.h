#pragma once

namespace te
{
    /** Interface that prevents copies be made of any type that implements it. */
    class INonCopyable
    {
    protected:
        INonCopyable() = default;
        ~INonCopyable() = default;

    private:
        INonCopyable(const INonCopyable&) = delete;
        INonCopyable& operator=(const INonCopyable&) = delete;
    };
}
