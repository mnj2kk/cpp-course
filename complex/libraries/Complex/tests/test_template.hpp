#ifndef TEST_TEMPLATE_HPP
#define TEST_TEMPLATE_HPP

#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <random>

#include "Complex.hpp"

double get_rand();

Complex get_random_complex();

#define COMPARE_EPS(actual, expected) REQUIRE_THAT(actual, Catch::Matchers::WithinAbs(expected, 1e-12))

#endif
