// tests/type_traits/test_type_traits.cpp
// Compile (from project root):
//   g++ -std=gnu++17 -Wall -Wextra -Wpedantic -I include tests/type_traits/test_type_traits.cpp -o test_type_traits
// Run:  ./test_type_traits

#include <iostream>
#include <string>
#include <mystl/type_traits/type_traits.hpp>

using namespace mystl;

// ------------------------------
// Helper types for tests
// ------------------------------
struct TrivialPod {
    int a;
    double b;
};

struct NonTrivialCopy {
    NonTrivialCopy() = default;
    NonTrivialCopy(const NonTrivialCopy&) {} // user-defined copy => typically NOT trivially copyable
};

struct HasValueType {
    using value_type = int;
};

struct NoValueType {};

// Detection idiom demo using mystl::void_t
template <typename, typename = mystl::void_t<>>
struct has_value_type : mystl::false_type {};

template <typename T>
struct has_value_type<T, mystl::void_t<typename T::value_type>> : mystl::true_type {};

// enable_if demo functions
template <typename T, mystl::enable_if_t<mystl::is_integral_v<T>, int> = 0>
const char* classify(T) {
    return "integral";
}

template <typename T, mystl::enable_if_t<!mystl::is_integral_v<T>, int> = 0>
const char* classify(T) {
    return "not integral";
}

int main() {
    std::cout << std::boolalpha;

    // ============================================================
    // 1) integral_constant / true_type / false_type / bool_constant
    // ============================================================
    {
        //compile time tests
        static_assert(integral_constant<int, 7>::value == 7);
        static_assert(true_type::value == true);
        static_assert(false_type::value == false);
        static_assert(bool_constant<true>::value == true);
        static_assert(bool_constant<false>::value == false);

        //run time tests
        std::cout << "[integral_constant]\n";
        std::cout << "  integral_constant<int,7>::value = " << integral_constant<int, 7>::value << "\n";
        std::cout << "  true_type::value = " << true_type::value << "\n";
        std::cout << "  false_type::value = " << false_type::value << "\n";
    }

    // =======================
    // 2) is_same / is_same_v
    // =======================
    {
        static_assert(is_same<int, int>::value);
        static_assert(!is_same<int, long>::value);
        static_assert(is_same_v<char, char>);
        static_assert(!is_same_v<char, signed char>);

        std::cout << "\n[is_same]\n";
        std::cout << "  is_same_v<int,int> = " << is_same_v<int, int> << "\n";
        std::cout << "  is_same_v<int,long> = " << is_same_v<int, long> << "\n";
    }

    // =====================================
    // 3) remove_const / remove_volatile / cv
    // =====================================
    {
        static_assert(is_same_v<remove_const_t<const int>, int>);
        static_assert(is_same_v<remove_const_t<int>, int>);

        static_assert(is_same_v<remove_volatile_t<volatile int>, int>);
        static_assert(is_same_v<remove_volatile_t<int>, int>);

        static_assert(is_same_v<remove_cv_t<const volatile int>, int>);
        static_assert(is_same_v<remove_cv_t<const int>, int>);
        static_assert(is_same_v<remove_cv_t<volatile int>, int>);
        static_assert(is_same_v<remove_cv_t<int>, int>);

        std::cout << "\n[remove_cv]\n";
        std::cout << "  remove_cv_t<const volatile int> is int? "
                  << is_same_v<remove_cv_t<const volatile int>, int> << "\n";
    }

    // ============================
    // 4) remove_reference / add_*_reference
    // ============================
    {
        static_assert(is_same_v<remove_reference_t<int&>, int>);
        static_assert(is_same_v<remove_reference_t<int&&>, int>);
        static_assert(is_same_v<remove_reference_t<const int&>, const int>);

        static_assert(is_same_v<add_lvalue_reference_t<int>, int&>);
        static_assert(is_same_v<add_rvalue_reference_t<int>, int&&>);

        std::cout << "\n[reference ops]\n";
        std::cout << "  remove_reference_t<int&&> is int? "
                  << is_same_v<remove_reference_t<int&&>, int> << "\n";
        std::cout << "  add_lvalue_reference_t<int> is int&? "
                  << is_same_v<add_lvalue_reference_t<int>, int&> << "\n";
        std::cout << "  add_rvalue_reference_t<int> is int&&? "
                  << is_same_v<add_rvalue_reference_t<int>, int&&> << "\n";
    }

    // ==========================================
    // 5) is_lvalue_reference / is_rvalue_reference / is_reference
    // ==========================================
    {
        static_assert(is_lvalue_reference_v<int&>);
        static_assert(!is_lvalue_reference_v<int>);
        static_assert(is_rvalue_reference_v<int&&>);
        static_assert(!is_rvalue_reference_v<int>);
        static_assert(is_reference_v<int&>);
        static_assert(is_reference_v<int&&>);
        static_assert(!is_reference_v<int>);

        std::cout << "\n[is_reference]\n";
        std::cout << "  is_reference_v<int&> = " << is_reference_v<int&> << "\n";
        std::cout << "  is_reference_v<int&&> = " << is_reference_v<int&&> << "\n";
        std::cout << "  is_reference_v<int> = " << is_reference_v<int> << "\n";
    }

    // ======================
    // 6) is_pointer
    // ======================
    {
        static_assert(is_pointer_v<int*>);
        static_assert(is_pointer_v<int* const>);
        static_assert(is_pointer_v<int* volatile>);
        static_assert(is_pointer_v<int* const volatile>);
        static_assert(!is_pointer_v<int>);
        static_assert(!is_pointer_v<int&>);

        std::cout << "\n[is_pointer]\n";
        std::cout << "  is_pointer_v<int*> = " << is_pointer_v<int*> << "\n";
        std::cout << "  is_pointer_v<int> = " << is_pointer_v<int> << "\n";
    }

    // ======================
    // 7) conditional
    // ======================
    {
        static_assert(is_same_v<conditional_t<true, int, double>, int>);
        static_assert(is_same_v<conditional_t<false, int, double>, double>);

        std::cout << "\n[conditional]\n";
        std::cout << "  conditional_t<true,int,double> is int? "
                  << is_same_v<conditional_t<true, int, double>, int> << "\n";
        std::cout << "  conditional_t<false,int,double> is double? "
                  << is_same_v<conditional_t<false, int, double>, double> << "\n";
    }

    // ======================
    // 8) enable_if (SFINAE)
    // ======================
    {
        std::cout << "\n[enable_if]\n";
        std::cout << "  classify(10) -> " << classify(10) << "\n";
        std::cout << "  classify(3.14) -> " << classify(3.14) << "\n";
        std::cout << "  classify('a') -> " << classify('a') << "\n";
    }

    // ======================
    // 9) is_integral
    // ======================
    {
        static_assert(is_integral_v<int>);
        static_assert(is_integral_v<const unsigned long long>);
        static_assert(is_integral_v<char>);
        static_assert(!is_integral_v<float>);
        static_assert(!is_integral_v<double>);
        static_assert(!is_integral_v<int*>);

        std::cout << "\n[is_integral]\n";
        std::cout << "  is_integral_v<int> = " << is_integral_v<int> << "\n";
        std::cout << "  is_integral_v<double> = " << is_integral_v<double> << "\n";
        std::cout << "  is_integral_v<int*> = " << is_integral_v<int*> << "\n";
    }

    // ======================
    // 10) void_t (detection)
    // ======================
    {
        static_assert(has_value_type<HasValueType>::value);
        static_assert(!has_value_type<NoValueType>::value);

        std::cout << "\n[void_t / detection]\n";
        std::cout << "  has_value_type<HasValueType> = " << has_value_type<HasValueType>::value << "\n";
        std::cout << "  has_value_type<NoValueType> = " << has_value_type<NoValueType>::value << "\n";
    }

    // ==========================================
    // 11) negation / conjunction / disjunction
    // ==========================================
    {
        using T = true_type;
        using F = false_type;

        static_assert(negation<F>::value);
        static_assert(!negation<T>::value);

        static_assert(conjunction<>::value); // empty AND => true
        static_assert(conjunction<T, T>::value);
        static_assert(!conjunction<T, F>::value);

        static_assert(!disjunction<>::value); // empty OR => false
        static_assert(disjunction<F, T>::value);
        static_assert(!disjunction<F, F>::value);

        std::cout << "\n[logic traits]\n";
        std::cout << "  negation<false_type> = " << negation<false_type>::value << "\n";
        std::cout << "  conjunction<true,true> = " << conjunction<true_type, true_type>::value << "\n";
        std::cout << "  conjunction<true,false> = " << conjunction<true_type, false_type>::value << "\n";
        std::cout << "  disjunction<false,true> = " << disjunction<false_type, true_type>::value << "\n";
        std::cout << "  disjunction<false,false> = " << disjunction<false_type, false_type>::value << "\n";
    }

    // ======================
    // 12) is_trivially_copyable
    // ======================
    {
        static_assert(is_trivially_copyable_v<int>);
        static_assert(is_trivially_copyable_v<TrivialPod>);
        static_assert(!is_trivially_copyable_v<NonTrivialCopy>);

        std::cout << "\n[is_trivially_copyable]\n";
        std::cout << "  is_trivially_copyable_v<int> = " << is_trivially_copyable_v<int> << "\n";
        std::cout << "  is_trivially_copyable_v<TrivialPod> = " << is_trivially_copyable_v<TrivialPod> << "\n";
        std::cout << "  is_trivially_copyable_v<NonTrivialCopy> = " << is_trivially_copyable_v<NonTrivialCopy> << "\n";
    }

    std::cout << "\nAll type_traits tests passed (compile-time + runtime prints).\n";
    return 0;
}
