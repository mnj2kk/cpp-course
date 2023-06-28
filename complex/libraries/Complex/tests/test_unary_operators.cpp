#include <catch2/catch_test_macros.hpp>

#include "Complex.hpp"
#include "test_template.hpp"

namespace {
const int NUMBER_TESTS = 993;
}

TEST_CASE("Conjugate test") {
    for (int _ = 0; _ < NUMBER_TESTS; ++_) {
        Complex _complex   = get_random_complex();
        Complex _conjugate = ~_complex;
        COMPARE_EPS(_conjugate.real(), _complex.real());
        COMPARE_EPS(_conjugate.imag(), -_complex.imag());
    }
}

TEST_CASE("Unary minus test") {
    for (int _ = 0; _ < NUMBER_TESTS; ++_) {
        Complex _complex   = get_random_complex();
        Complex _conjugate = -_complex;
        COMPARE_EPS(_conjugate.real(), -_complex.real());
        COMPARE_EPS(_conjugate.imag(), -_complex.imag());
    }
}
