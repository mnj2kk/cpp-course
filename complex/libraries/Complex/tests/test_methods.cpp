#include <catch2/catch_test_macros.hpp>

#include "Complex.hpp"
#include "test_template.hpp"

namespace {
double sum_of_squares(const Complex& complex) {
    return complex.real() * complex.real() + complex.imag() * complex.imag();
}
const int TEST_NUMBERS = 993;
}  // namespace

TEST_CASE(".abs() test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _complex = get_random_complex();
        COMPARE_EPS(_complex.abs(), std::sqrt(sum_of_squares(_complex)));
    }
}

TEST_CASE(".str() test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _complex = get_random_complex();
        const std::string _expected =
            std::to_string(_complex.real()) + " + " + std::to_string(_complex.imag()) + " * i";
        REQUIRE(_expected == _complex.str());
    }
}

TEST_CASE("Multiply by conjugate test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _complex = get_random_complex();
        COMPARE_EPS(_complex.multiply_by_conjugate(), sum_of_squares(_complex));
    }
}
