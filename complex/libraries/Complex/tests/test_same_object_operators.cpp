#include <catch2/catch_test_macros.hpp>

#include "Complex.hpp"
#include "test_template.hpp"

namespace {
const int TEST_NUMBERS = 993;
}  // namespace

TEST_CASE("Same object equal to operator random") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _complex = get_random_complex();
        REQUIRE(_complex == _complex);
        REQUIRE_FALSE(_complex != _complex);
    }
}

TEST_CASE("Same object addition assignment operator random") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _complex = get_random_complex();
        double _real     = _complex.real() * 2;
        double _imagine  = _complex.imag() * 2;
        _complex += _complex;
        COMPARE_EPS(_complex.real(), _real);
        COMPARE_EPS(_complex.imag(), _imagine);
    }
}

TEST_CASE("Same object subtraction assignment operator random") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _complex = get_random_complex();
        _complex -= _complex;
        COMPARE_EPS(_complex.real(), 0.);
        COMPARE_EPS(_complex.imag(), 0.);
    }
}

TEST_CASE("Same object multiply assignment operator random") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _complex    = get_random_complex();
        double _old_real    = _complex.real();
        double _old_imagine = _complex.imag();
        double _real        = _old_real * _old_real - _old_imagine * _old_imagine;
        double _imagine     = 2 * _old_real * _old_imagine;
        _complex *= _complex;
        COMPARE_EPS(_complex.real(), _real);
        COMPARE_EPS(_complex.imag(), _imagine);
    }
}

TEST_CASE("Same object division assignment operator random") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _complex = get_random_complex();
        _complex /= _complex;
        COMPARE_EPS(_complex.real(), 1.);
        COMPARE_EPS(_complex.imag(), 0.);
    }
}
