#pragma once

#include <type_traits>

namespace AeroJet
{
    template<typename T>
    inline constexpr typename std::remove_reference_t<T>&& moveTemp(T&& object)
    {
        using ReturnType = typename std::remove_reference_t<T>;

        static_assert(std::is_lvalue_reference_v<T>);
        static_assert(!std::is_same_v<ReturnType&, const ReturnType&>);

        return (ReturnType&&)object;
    }
} // namespace AeroJet
