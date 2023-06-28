#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <map>
#include <sstream>
#include <stdexcept>

#include "Expressions.hpp"
#include "test_template.hpp"

namespace {
const int NUMBER_TESTS = 993;

void random_complex_test(const Expression& expression,
                         const std::function<Complex(std::map<std::string, Complex>)>& validate,
                         const std::vector<std::string>& names) {
    for (int _ = 0; _ < NUMBER_TESTS; ++_) {
        std::map<std::string, Complex> _variables;
        for (const std::string& name : names) {
            _variables[name] = get_random_complex();
        }
        REQUIRE_NOTHROW(expression.eval(_variables));
        const Complex _expr_result     = expression.eval(_variables);
        const Complex _validate_result = validate(_variables);
        COMPARE_EPS(_validate_result.real(), _expr_result.real());
        COMPARE_EPS(_validate_result.imag(), _expr_result.imag());
    }
}
void throw_validate(const Expression& expression, const std::vector<std::string>& names) {
    std::size_t _cnt = 1 << names.size();
    for (std::size_t mask = 0; mask + 1 < _cnt; ++mask) {
        std::map<std::string, Complex> _variables;
        for (std::size_t i = 0; i < names.size(); ++i) {
            if ((mask & (1 << i)) != 0) {
                _variables[names[i]] = get_random_complex();
            }
        }
        REQUIRE_THROWS_AS(expression.eval(_variables), std::out_of_range);
    }
}

std::vector<Complex> get_consts(std::size_t N) {
    std::vector<Complex> _result;
    for (std::size_t i = 0; i < N; ++i) {
        _result.emplace_back(get_random_complex());
    }
    return _result;
}

#define TEST_EXPRESSION(...)                                \
    const std::vector<std::string> _variables{__VA_ARGS__}; \
    const Subtract _diff = Subtract(_expr, *_expr.clone()); \
    random_complex_test(_variables, _diff);                 \
    random_complex_test(_expr, _validate, _variables);      \
    throw_validate(_expr, _variables)

#define STRING_TEST()                 \
    {                                 \
        REQUIRE(_str == _expr.str()); \
        std::stringstream _out;       \
        _out << _expr;                \
        REQUIRE(_str == _out.str());  \
    }

}  // namespace

TEST_CASE("Basic expression test №1") {
    std::vector<Complex> _complex = get_consts(1);
    const Subtract _expr          = Subtract(Multiply(Const(_complex[0]), Add(Variable("x"), Variable("y"))),
                                             Divide(Variable("x"), Subtract(Variable("x"), Conjugate(Variable("y")))));
    const std::string _str        = "(((" + _complex[0].str() + ") * (x + y)) - (x / (x - ~(y))))";
    STRING_TEST()
    const auto _validate = [&_complex](const std::map<std::string, Complex>& variables) {
        return (_complex[0] * (variables.at("x") + variables.at("y"))) -
               variables.at("x") / (variables.at("x") - ~variables.at("y"));
    };
    TEST_EXPRESSION("x", "y");
}

TEST_CASE("Basic expression test №2") {
    std::vector<Complex> _complex = get_consts(1);
    const Conjugate _expr =
        Conjugate(Divide(Subtract(Divide(Variable("x"), Variable("y")), Multiply(Const(_complex[0]), Variable("z"))),
                         Add(Negate(Variable("z")), ~Variable("y"))));
    const std::string _str = "~((((x / y) - ((" + _complex[0].str() + ") * z)) / (-(z) + ~(y))))";
    STRING_TEST()
    const auto _validate = [&_complex](const std::map<std::string, Complex>& variables) {
        return ~((variables.at("x") / variables.at("y") - _complex[0] * variables.at("z")) /
                 (-variables.at("z") + ~variables.at("y")));
    };
    TEST_EXPRESSION("x", "y", "z");
}

TEST_CASE("Basic expression test №3") {
    std::vector<Complex> _complex = get_consts(2);
    const Negate _expr            = Negate(Conjugate(
                   Add(Multiply(Const(_complex[0]), Divide(Variable("x"), Variable("y"))),
                       Subtract(Divide(Conjugate(Subtract(Variable("x"), Variable("z"))), Const(_complex[1])), Variable("z")))));
    const std::string _str =
        "-(~((((" + _complex[0].str() + ") * (x / y)) + ((~((x - z)) / (" + _complex[1].str() + ")) - z))))";
    STRING_TEST()
    const auto _validate = [&_complex](const std::map<std::string, Complex>& variables) {
        return -(~(_complex[0] * (variables.at("x") / variables.at("y")) +
                   ((~(variables.at("x") - variables.at("z"))) / _complex[1] - variables.at("z"))));
    };
    TEST_EXPRESSION("x", "y", "z");
}

TEST_CASE("Basic expression test №4") {
    std::vector<Complex> _complex = get_consts(2);
    const Add _expr =
        Add(Conjugate(Divide(Variable("y"), Multiply(Const(_complex[0]), Subtract(Variable("x"), Variable("y"))))),
            Negate(Add(Variable("x"), Const(_complex[1]))));
    const std::string _str =
        "(~((y / ((" + _complex[0].str() + ") * (x - y)))) + -((x + (" + _complex[1].str() + "))))";
    STRING_TEST()
    const auto _validate = [&_complex](const std::map<std::string, Complex>& variables) {
        return ~(variables.at("y") / (_complex[0] * (variables.at("x") - variables.at("y")))) +
               -(variables.at("x") + _complex[1]);
    };
    TEST_EXPRESSION("x", "y");
}

TEST_CASE("Basic expression test №5") {
    std::vector<Complex> _complex = get_consts(4);
    const Multiply _expr          = Add(Conjugate(Const(_complex[0])), Variable("x")) *
                           Divide(Subtract(Variable("x") - Const(_complex[1])),
                                  Negate(Variable("x") / (Multiply(Const(_complex[2]), Const(_complex[3])))));
    const std::string _str = "((~((" + _complex[0].str() + ")) + x) * ((x - (" + _complex[1].str() + ")) / -((x / ((" +
                             _complex[2].str() + ") * (" + _complex[3].str() + "))))))";
    STRING_TEST()
    const auto _validate = [&_complex](const std::map<std::string, Complex>& variables) {
        return (~_complex[0] + variables.at("x")) *
               ((variables.at("x") - _complex[1]) / -(variables.at("x") / (_complex[2] * _complex[3])));
    };
    TEST_EXPRESSION("x");
}

TEST_CASE("Basic expression test №6") {
    std::vector<Complex> _complex = get_consts(2);
    const Divide _expr = Divide(Const(_complex[0]) * Add(Variable("y"), Multiply(Variable("x"), Const(_complex[1]))),
                                -Subtract(Variable("y"), Conjugate(Const(_complex[1]))));
    const std::string _str = "(((" + _complex[0].str() + ") * (y + (x * (" + _complex[1].str() + ")))) / -((y - ~((" +
                             _complex[1].str() + ")))))";
    STRING_TEST()
    const auto _validate = [&_complex](const std::map<std::string, Complex>& variables) {
        return (_complex[0] * (variables.at("y") + variables.at("x") * _complex[1])) /
               -(variables.at("y") - ~_complex[1]);
    };
    TEST_EXPRESSION("x", "y");
}
