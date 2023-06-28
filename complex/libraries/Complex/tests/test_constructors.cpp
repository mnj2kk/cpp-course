#include <catch2/catch_test_macros.hpp>

#include "Complex.hpp"
#include "test_template.hpp"

TEST_CASE("Default construction") {
    const Complex complex{};
    COMPARE_EPS(complex.real(), 0.);
    COMPARE_EPS(complex.imag(), 0.);
}

TEST_CASE("Real part construction") {
    const Complex complex{2.};
    COMPARE_EPS(complex.real(), 2.);
    COMPARE_EPS(complex.imag(), 0.);
}

TEST_CASE("Full complex number construction") {
    const Complex complex{1.3, 3.7};
    COMPARE_EPS(complex.real(), 1.3);
    COMPARE_EPS(complex.imag(), 3.7);
}

TEST_CASE("Copy construct") {
    Complex complex{1.3, 3.7};
    const Complex copied{complex};
    REQUIRE(complex == copied);
    REQUIRE_FALSE(complex != copied);
    complex *= 2.;
    REQUIRE(complex != copied);
    COMPARE_EPS(complex.real(), copied.real() + copied.real());
    COMPARE_EPS(complex.imag(), copied.imag() + copied.imag());
}
