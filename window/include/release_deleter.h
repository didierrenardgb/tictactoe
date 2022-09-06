#pragma once

namespace ttt
{

template<class T>
class release_deleter
{
public:
    void operator()(T*& pointer) const noexcept
    {
        static_assert(0 < sizeof(T), "can't delete an incomplete type");
        if (pointer)
        {
            pointer->Release();
            pointer = nullptr;
        }
    }
};

} // namespace ttt
