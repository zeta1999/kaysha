#ifndef KAYSHA_DIVISION_HPP
#define KAYSHA_DIVISION_HPP
#include <type_traits>
#include "tag.hpp"
#include "differentiation.hpp"

namespace kaysha
{

template<typename Lhs, typename Rhs>
struct division: public kaysha_type
{
    using lhs_value_type = typename Lhs::value_type;
    using rhs_value_type = typename Rhs::value_type;
    using value_type =
        typename std::common_type<lhs_value_type, rhs_value_type>::type;

    constexpr division(const Lhs& l, const Rhs& r) noexcept: lhs(l), rhs(r) {}
    constexpr ~division() noexcept = default;
    constexpr division(division const&) noexcept = default;
    constexpr division(division &&)     noexcept = default;
    constexpr division& operator=(division const&) noexcept = default;
    constexpr division& operator=(division &&)     noexcept = default;

    constexpr value_type operator()(value_type x) noexcept
    {return lhs(x) / rhs(x);}

    Lhs lhs;
    Rhs rhs;
};

template<typename Lhs, typename Rhs>
struct differentiation<division<Lhs, Rhs>>
{
    static_assert(std::is_same<
        typename Lhs::value_type, typename Rhs::value_type>::value, "");

    using type = division<subtraction<
            multiply<differentiation<Lhs>, Rhs>,
            multiply<differentiation<Rhs>, Lhs>
            >,
            multiply<Rhs, Rhs>
        >;
};

template<typename Lhs, typename Rhs>
constexpr typename std::enable_if<
    is_kaysha_type<Lhs>::value && is_kaysha_type<Rhs>::value, division<Lhs, Rhs>
    >::type
operator/(const Lhs& l, const Rhs& r) noexcept
{
    return division<Lhs, Rhs>(l, r);
}

} // kaysha
#endif// KAYSHA_DIVISION_HPP