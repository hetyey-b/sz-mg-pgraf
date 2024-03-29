// Copyright (c) 2016-2017 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PFR_PRECISE_OPS_HPP
#define BOOST_PFR_PRECISE_OPS_HPP
#pragma once

#include "../detail/config.hpp"

#include "../detail/detectors.hpp"
#include "functors.hpp"
#include "core.hpp"
#include "io.hpp"

/// \file boost/pfr/precise/ops.hpp
/// Contains comparison operators and stream operators for types that do not have their own operators.
/// If type is comparable or streamable using it's own operator or it's conversion operator, then the original operator is used.
///
/// Just write \b using \b namespace \b ops; and operators will be available in scope.
///
/// \b Requires: C++17 or \constexprinit{C++14 constexpr aggregate intializable type}.
///
/// \rcast14
///
/// \b Example:
/// \code
///     #include <boost/pfr/precise/ops.hpp>
///     struct comparable_struct {      // No operators defined for that structure
///         int i; short s; char data[7]; bool bl; int a,b,c,d,e,f;
///     };
///     // ...
///
///     using namespace ops;
///
///     comparable_struct s1 {0, 1, "Hello", false, 6,7,8,9,10,11};
///     comparable_struct s2 {0, 1, "Hello", false, 6,7,8,9,10,11111};
///     assert(s1 < s2);
///     std::cout << s1 << std::endl; // Outputs: {0, 1, H, e, l, l, o, , , 0, 6, 7, 8, 9, 10, 11}
/// \endcode
///
/// \podops for other ways to define operators and more details.
///
/// \b This \b header \b contains:
namespace boost { namespace pfr {

namespace detail {

///////////////////// Helper typedef that it used by all the enable_not_*_comp_t
    template <template <class, class> class Detector, class T>
    using enable_not_comp_base_t = typename std::enable_if<
        not_appliable<Detector, T const&, T const&>::value,
        bool
    >::type;

///////////////////// std::enable_if_t like functions that enable only if types do not support operation and are PODs

    template <class T> using enable_not_eq_comp_t = enable_not_comp_base_t<comp_eq_detector, T>;
    template <class T> using enable_not_ne_comp_t = enable_not_comp_base_t<comp_ne_detector, T>;
    template <class T> using enable_not_lt_comp_t = enable_not_comp_base_t<comp_lt_detector, T>;
    template <class T> using enable_not_le_comp_t = enable_not_comp_base_t<comp_le_detector, T>;
    template <class T> using enable_not_gt_comp_t = enable_not_comp_base_t<comp_gt_detector, T>;
    template <class T> using enable_not_ge_comp_t = enable_not_comp_base_t<comp_ge_detector, T>;

    template <class Stream, class Type>
    using enable_not_ostreamable_t = typename std::enable_if<
        not_appliable<ostreamable_detector, Stream&, Type const&>::value,
        Stream&
    >::type;

    template <class Stream, class Type>
    using enable_not_istreamable_t = typename std::enable_if<
        not_appliable<istreamable_detector, Stream&, Type&>::value,
        Stream&
    >::type;
} // namespace detail

namespace ops {
#ifdef BOOST_PFR_DOXYGEN_INVOKED
    template <class T> bool operator==(const T& lhs, const T& rhs) noexcept;
    template <class T> bool operator!=(const T& lhs, const T& rhs) noexcept;
    template <class T> bool operator< (const T& lhs, const T& rhs) noexcept;
    template <class T> bool operator> (const T& lhs, const T& rhs) noexcept;
    template <class T> bool operator<=(const T& lhs, const T& rhs) noexcept;
    template <class T> bool operator>=(const T& lhs, const T& rhs) noexcept;

    template <class Char, class Traits, class T>
    std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& out, const T& value);

    template <class Char, class Traits, class T>
    std::basic_istream<Char, Traits>& operator>>(std::basic_istream<Char, Traits>& in, T& value);

    /// \brief helper function for Boost
    template <class T> std::size_t hash_value(const T& value) noexcept;
#else
    template <class T>
    static detail::enable_not_eq_comp_t<T> operator==(const T& lhs, const T& rhs) noexcept {
        return equal_to<T>{}(lhs, rhs);
    }

    template <class T>
    static detail::enable_not_ne_comp_t<T> operator!=(const T& lhs, const T& rhs) noexcept {
        return not_equal<T>{}(lhs, rhs);
    }

    template <class T>
    static detail::enable_not_lt_comp_t<T> operator<(const T& lhs, const T& rhs) noexcept {
        return less<T>{}(lhs, rhs);
    }

    template <class T>
    static detail::enable_not_gt_comp_t<T> operator>(const T& lhs, const T& rhs) noexcept {
        return greater<T>{}(lhs, rhs);
    }

    template <class T>
    static detail::enable_not_le_comp_t<T> operator<=(const T& lhs, const T& rhs) noexcept {
        return less_equal<T>{}(lhs, rhs);
    }

    template <class T>
    static detail::enable_not_ge_comp_t<T> operator>=(const T& lhs, const T& rhs) noexcept {
        return greater_equal<T>{}(lhs, rhs);
    }

    template <class Char, class Traits, class T>
    static detail::enable_not_ostreamable_t<std::basic_ostream<Char, Traits>, T> operator<<(std::basic_ostream<Char, Traits>& out, const T& value) {
        boost::pfr::write(out, value);
        return out;
    }

    template <class Char, class Traits, class T>
    static detail::enable_not_istreamable_t<std::basic_istream<Char, Traits>, T> operator>>(std::basic_istream<Char, Traits>& in, T& value) {
        boost::pfr::read(in, value);
        return in;
    }

    template <class T>
    static std::enable_if_t<std::is_pod<T>::value, std::size_t> hash_value(const T& value) noexcept {
        return hash<T>{}(value);
    }

#endif
} // namespace ops

}} // namespace boost::pfr

#endif // BOOST_PFR_PRECISE_OPS_HPP
