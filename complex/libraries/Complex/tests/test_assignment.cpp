#include <catch2/catch_test_macros.hpp>

#include "Complex.hpp"
#include "test_template.hpp"

namespace {
const int TEST_NUMBERS = 993;
}  // namespace

#define OPERATOR_CREATION(op)               \
    Complex _first  = get_random_complex(); \
    Complex _second = get_random_complex(); \
    Complex _actual = _first;               \
    _actual op _second;

#define TEST_OPERATOR(first_op, default_op)                               \
    OPERATOR_CREATION(first_op)                                           \
    COMPARE_EPS(_actual.real(), _first.real() default_op _second.real()); \
    COMPARE_EPS(_actual.imag(), _first.imag() default_op _second.imag())

TEST_CASE("Addition assignment operator test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        TEST_OPERATOR(+=, +);
        Complex _reversed = _second;
        _reversed += _first;
        REQUIRE(_actual == _reversed);
    }
}

TEST_CASE("Subtraction assignment operator test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        TEST_OPERATOR(-=, -);
        Complex _reversed = _second;
        _reversed -= _first;
        COMPARE_EPS(_reversed.real(), -_actual.real());
        COMPARE_EPS(_reversed.imag(), -_actual.imag());
    }
}

TEST_CASE("Multiply assignment operator test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _first           = get_random_complex();
        Complex _second          = get_random_complex();
        double _expected_real    = _first.real() * _second.real() - _first.imag() * _second.imag();
        double _expected_imagine = _first.real() * _second.imag() + _first.imag() * _second.real();
        Complex _actual          = _first;
        _actual *= _second;
        COMPARE_EPS(_actual.real(), _expected_real);
        COMPARE_EPS(_actual.imag(), _expected_imagine);
        Complex _reversed = _second;
        _reversed *= _first;
        COMPARE_EPS(_reversed.real(), _expected_real);
        COMPARE_EPS(_reversed.imag(), _expected_imagine);
    }
}

TEST_CASE("Division assignment operator test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _first           = get_random_complex();
        Complex _second          = get_random_complex();
        double _soq_second       = _second.real() * _second.real() + _second.imag() * _second.imag();
        double _expected_real    = (_first.real() * _second.real() + _first.imag() * _second.imag()) / _soq_second;
        double _expected_imagine = (_first.imag() * _second.real() - _first.real() * _second.imag()) / _soq_second;
        Complex _actual          = _first;
        _actual /= _second;
        COMPARE_EPS(_actual.real(), _expected_real);
        COMPARE_EPS(_actual.imag(), _expected_imagine);
    }
}

TEST_CASE("Multiply by double assignment operator test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _complex = get_random_complex();
        double _value    = get_rand();
        Complex _actual  = _complex;
        _actual *= _value;
        COMPARE_EPS(_actual.real(), _complex.real() * _value);
        COMPARE_EPS(_actual.imag(), _complex.imag() * _value);
    }
}

TEST_CASE("Division by double assignment operator test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _complex = get_random_complex();
        double _value    = get_rand();
        Complex _actual  = _complex;
        _actual /= _value;
        COMPARE_EPS(_actual.real(), _complex.real() / _value);
        COMPARE_EPS(_actual.imag(), _complex.imag() / _value);
    }
}
