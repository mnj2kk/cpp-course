#include "test_template.hpp"

namespace {
thread_local std::mt19937 _rnd(std::random_device{}());
}  // namespace

double get_rand() {
    return 2e4 * (std::uniform_real_distribution<double>{}(_rnd)-0.5);
}

Complex get_random_complex() {
    return {get_rand(), get_rand()};
}
