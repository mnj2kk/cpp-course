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

void random_complex_test(const std::vector<std::string>& names, const Expression& expression) {
    for (int i = 0; i < 993; ++i) {
        std::map<std::string, Complex> variables;
        for (const std::string& name : names) {
            variables[name] = get_random_complex();
        }
        const Complex _result = expression.eval(variables);
        COMPARE_EPS(_result.real(), 0.);
        COMPARE_EPS(_result.imag(), 0.);
    }
}
