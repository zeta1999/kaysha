#ifndef KAYSHA_MULTIPLY_HPP
#define KAYSHA_MULTIPLY_HPP
#include <type_traits>
#include "tag.hpp"
#include "differentiation.hpp"
#include "addition.hpp"

namespace kaysha
{

template<typename Lhs, typename Rhs>
struct multiplication: public kaysha_type
{
    using lhs_value_type = typename Lhs::value_type;
    using rhs_value_type = typename Rhs::value_type;
    using value_type =
        typename std::common_type<lhs_value_type, rhs_value_type>::type;

    constexpr multiplication(const Lhs& l, const Rhs& r) noexcept: lhs(l), rhs(r) {}
    constexpr ~multiplication() noexcept = default;
    constexpr multiplication(multiplication const&) noexcept = default;
    constexpr multiplication(multiplication &&)     noexcept = default;
    constexpr multiplication& operator=(multiplication const&) noexcept = default;
    constexpr multiplication& operator=(multiplication &&)     noexcept = default;

    constexpr value_type operator()(value_type x) noexcept
    {return lhs(x) * rhs(x);}

    Lhs lhs;
    Rhs rhs;
};

template<typename Lhs, typename Rhs>
struct differentiation<multiplication<Lhs, Rhs>>
{
    static_assert(std::is_same<
        typename Lhs::value_type, typename Rhs::value_type>::value, "");

    using type = addition<
            multiplication<differentiation<Lhs>, Rhs>,
            multiplication<differentiation<Rhs>, Lhs>
        >;
};

template<typename Lhs, typename Rhs>
constexpr typename std::enable_if<
    is_kaysha_type<Lhs>::value && is_kaysha_type<Rhs>::value, multiplication<Lhs, Rhs>
    >::type
operator*(const Lhs& l, const Rhs& r) noexcept
{
    return multiplication<Lhs, Rhs>(l, r);
}

} // kaysha
#endif// KAYSHA_MULTIPLY_HPP