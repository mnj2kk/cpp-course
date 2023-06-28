#include <catch2/catch_test_macros.hpp>

#include "Complex.hpp"
#include "test_template.hpp"

namespace {
double sum_of_squares(const Complex& complex) {
    return complex.real() * complex.real() + complex.imag() * complex.imag();
}
const int TEST_NUMBERS = 31;
}  // namespace

#define OPERATOR_CREATION(op)               \
    Complex _first  = get_random_complex(); \
    Complex _second = get_random_complex(); \
    Complex _actual = _first op _second

#define TEST_OPERATOR(op)                                         \
    OPERATOR_CREATION(op);                                        \
    COMPARE_EPS(_actual.real(), _first.real() op _second.real()); \
    COMPARE_EPS(_actual.imag(), _first.imag() op _second.imag())

TEST_CASE("Addition operator test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        TEST_OPERATOR(+);
    }
}

TEST_CASE("Subtraction operator test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        TEST_OPERATOR(-);
    }
}

TEST_CASE("Multiply operator test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        OPERATOR_CREATION(*);
        double _expected_real    = _first.real() * _second.real() - _first.imag() * _second.imag();
        double _expected_imagine = _first.real() * _second.imag() + _first.imag() * _second.real();
        COMPARE_EPS(_actual.real(), (_second * _first).real());
        COMPARE_EPS(_actual.imag(), (_second * _first).imag());
        COMPARE_EPS(_actual.real(), _expected_real);
        COMPARE_EPS(_actual.imag(), _expected_imagine);
    }
}

TEST_CASE("Division operator test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        OPERATOR_CREATION(/);
        double _soq_second       = sum_of_squares(_second);
        double _expected_real    = (_first.real() * _second.real() + _first.imag() * _second.imag()) / _soq_second;
        double _expected_imagine = (_first.imag() * _second.real() - _first.real() * _second.imag()) / _soq_second;
        COMPARE_EPS(_actual.real(), _expected_real);
        COMPARE_EPS(_actual.imag(), _expected_imagine);
    }
}

TEST_CASE("Multiply by double test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _complex    = get_random_complex();
        double _value       = get_rand();
        Complex _multiplied = _complex * _value;
        COMPARE_EPS(_multiplied.real(), (_value * _complex).real());
        COMPARE_EPS(_multiplied.imag(), (_value * _complex).imag());
        COMPARE_EPS(_multiplied.real(), _complex.real() * _value);
        COMPARE_EPS(_multiplied.imag(), _complex.imag() * _value);
    }
}

TEST_CASE("Division by double test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _complex = get_random_complex();
        double _value    = get_rand();
        Complex _divide  = _complex / _value;
        COMPARE_EPS(_divide.real(), _complex.real() / _value);
        COMPARE_EPS(_divide.imag(), _complex.imag() / _value);
    }
}

TEST_CASE("Division double by _complex number test") {
    for (int i = 0; i < TEST_NUMBERS; ++i) {
        Complex _complex         = get_random_complex();
        double _value            = get_rand();
        Complex _divide          = _value / _complex;
        double _soq_complex      = sum_of_squares(_complex);
        double _expected_real    = _value * _complex.real() / _soq_complex;
        double _expected_imagine = -_value * _complex.imag() / _soq_complex;
        COMPARE_EPS(_divide.real(), _expected_real);
        COMPARE_EPS(_divide.imag(), _expected_imagine);
    }
}
