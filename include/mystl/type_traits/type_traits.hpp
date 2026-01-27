/*
what are type traits? -> it is a feature in C++ that allows you to query or modify the properties of types at compile time.
when you use type traits, you can determine the properties of types at compile time.

use cases of type traits:
1. Conditional compilation: You can use type traits to enable or disable certain code paths based on type properties.
2. Template specialization: Type traits can help you create specialized implementations of templates based on type characteristics.
3. Type transformations: You can use type traits to transform types, such as removing constness or adding pointers.     
4. Static assertions: Type traits can be used in static assertions to enforce certain type properties at compile time.
    template<typename T,T val>
    struct integral_constant {
        static constexpr T value = val;
        using value_type = T;
        using type = integral_constant;

        constexpr operator value_type() const noexcept {
            return value;
        }

        constexpr value_type operator()() const noexcept {
            return value;
        }
    };
*/

#pragma once //this is a header guard to prevent multiple inclusions of this file.


namespace mystl {

    //Integral constant

    template <typename T,T v>
    struct integral_constant {

        static constexpr T value = v;
        using value_type = T;
        using type = integral_constant;

        constexpr operator value_type() const noexcept { return value;}
        constexpr value_type operator()() const noexcept { return value; }

        using true_type = integral_constant<bool,true>;
        using false_type = integral_constant<bool,false>;

    };
}