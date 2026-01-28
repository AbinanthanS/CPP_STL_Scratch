#pragma once

namespace mystl {

//============================================================
// 1) integral_constant / bool_constant
//============================================================
template <typename T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;

    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

using true_type  = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <bool B>
using bool_constant = integral_constant<bool, B>;

//============================================================
// 2) enable_if
//============================================================
template <bool, typename T = void>
struct enable_if { };

template <typename T>
struct enable_if<true, T> { using type = T; };

template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

//============================================================
// 3) conditional
//============================================================
template <bool B, typename T, typename F>
struct conditional { using type = T; };

template <typename T, typename F>
struct conditional<false, T, F> { using type = F; };

template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

//============================================================
// 4) is_same
//============================================================
template <typename T, typename U>
struct is_same : false_type { };

template <typename T>
struct is_same<T, T> : true_type { };

template <typename T, typename U>
inline constexpr bool is_same_v = is_same<T, U>::value;

//============================================================
// 5) remove_const / remove_volatile / remove_cv
//============================================================
template <typename T>
struct remove_const { using type = T; };

template <typename T>
struct remove_const<const T> { using type = T; };

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <typename T>
struct remove_volatile { using type = T; };

template <typename T>
struct remove_volatile<volatile T> { using type = T; };

template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

template <typename T>
struct remove_cv {
    using type = remove_const_t<remove_volatile_t<T>>;
};

template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

//============================================================
// 6) remove_reference
//============================================================
template <typename T>
struct remove_reference { using type = T; };

template <typename T>
struct remove_reference<T&> { using type = T; };

template <typename T>
struct remove_reference<T&&> { using type = T; };

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

//============================================================
// 7) add_lvalue_reference / add_rvalue_reference
// (Good enough for move/forward + declval-style utilities later)
//============================================================
template <typename T>
struct add_lvalue_reference { using type = T&; };

template <typename T>
struct add_lvalue_reference<T&> { using type = T&; };

template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

template <typename T>
struct add_rvalue_reference { using type = T&&; };

template <typename T>
struct add_rvalue_reference<T&&> { using type = T&&; };

template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

//============================================================
// 8) is_reference
//============================================================
template <typename T>
struct is_lvalue_reference : false_type { };

template <typename T>
struct is_lvalue_reference<T&> : true_type { };

template <typename T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

template <typename T>
struct is_rvalue_reference : false_type { };

template <typename T>
struct is_rvalue_reference<T&&> : true_type { };

template <typename T>
inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

template <typename T>
struct is_reference : bool_constant<is_lvalue_reference_v<T> || is_rvalue_reference_v<T>> { };

template <typename T>
inline constexpr bool is_reference_v = is_reference<T>::value;

//============================================================
// 9) is_pointer
//============================================================
template <typename T>
struct is_pointer : false_type { };

template <typename T>
struct is_pointer<T*> : true_type { };

template <typename T>
struct is_pointer<T* const> : true_type { };

template <typename T>
struct is_pointer<T* volatile> : true_type { };

template <typename T>
struct is_pointer<T* const volatile> : true_type { };

template <typename T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;

//============================================================
// 10) void_t  (for SFINAE / detection idiom; iterator_traits later)
//============================================================
template <typename...>
using void_t = void;

//============================================================
// 11) negation / conjunction / disjunction (C++17 style)
//============================================================
template <typename B>
struct negation : bool_constant<!static_cast<bool>(B::value)> { };

template <typename...>
struct conjunction : true_type { };

template <typename B1>
struct conjunction<B1> : B1 { };

template <typename B1, typename... BN>
struct conjunction<B1, BN...>
    : conditional_t<static_cast<bool>(B1::value), conjunction<BN...>, B1> { };

template <typename...>
struct disjunction : false_type { };

template <typename B1>
struct disjunction<B1> : B1 { };

template <typename B1, typename... BN>
struct disjunction<B1, BN...>
    : conditional_t<static_cast<bool>(B1::value), B1, disjunction<BN...>> { };

//============================================================
// 12) is_integral (needed for overload gating + algorithm choices)
//============================================================
template <typename T>
struct is_integral : false_type { };

// base integral types
template <> struct is_integral<bool>               : true_type { };
template <> struct is_integral<char>               : true_type { };
template <> struct is_integral<signed char>        : true_type { };
template <> struct is_integral<unsigned char>      : true_type { };
template <> struct is_integral<wchar_t>            : true_type { };
template <> struct is_integral<char16_t>           : true_type { };
template <> struct is_integral<char32_t>           : true_type { };
template <> struct is_integral<short>              : true_type { };
template <> struct is_integral<unsigned short>     : true_type { };
template <> struct is_integral<int>                : true_type { };
template <> struct is_integral<unsigned int>       : true_type { };
template <> struct is_integral<long>               : true_type { };
template <> struct is_integral<unsigned long>      : true_type { };
template <> struct is_integral<long long>          : true_type { };
template <> struct is_integral<unsigned long long> : true_type { };

// cv-qualified integrals are integral
template <typename T> struct is_integral<const T>          : is_integral<T> { };
template <typename T> struct is_integral<volatile T>       : is_integral<T> { };
template <typename T> struct is_integral<const volatile T> : is_integral<T> { };

template <typename T>
inline constexpr bool is_integral_v = is_integral<T>::value;

//============================================================
// 13) is_trivially_copyable (compiler builtin; used for fast paths)
//============================================================
// NOTE: Most major compilers support __is_trivially_copyable(T).
template <typename T>
struct is_trivially_copyable
    : bool_constant<__is_trivially_copyable(T)> { };

template <typename T>
inline constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;

} // namespace mystl
