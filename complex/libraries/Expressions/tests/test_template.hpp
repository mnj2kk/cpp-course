#ifndef COMPLEX_TEMPLATE_HPP
#define COMPLEX_TEMPLATE_HPP

#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <random>

#include "Complex.hpp"
#include "Expressions.hpp"

double get_rand();

Complex get_random_complex();

void random_complex_test(const std::vector<std::string>& names, const Expression& expression);

#define COMPARE_EPS(actual, expected) REQUIRE_THAT(actual, Catch::Matchers::WithinAbs(expected, 1e-14))

#endif
