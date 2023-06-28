#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <stdexcept>

#include "Expressions.hpp"
#include "test_template.hpp"

namespace {

template <typename T>
T get_simple_binary() {
    return {Const(2.8, 3.1), Variable("x")};
}

std::string get_simple_binary_str(const std::string& op) {
    return "((" + Complex(2.8, 3.1).str() + ")" + op + "x)";
}

std::string get_simple_unary_str(const std::string& op) {
    return op + "(x)";
}

#define STRING_TEST(original_string, cur_string, original, cur) \
    REQUIRE((original_string) == (cur_string));                 \
    {                                                           \
        std::stringstream _original;                            \
        _original << (original);                                \
        std::stringstream _cur;                                 \
        _cur << (cur);                                          \
        REQUIRE(_original.str() == _cur.str());                 \
    }

#define OPERATOR_CREATION(type, name, op)               \
    const auto name        = get_simple_binary<type>(); \
    const std::string _str = get_simple_binary_str(op); \
    REQUIRE(_str == (name).str())

#define OPERATOR_BINARY_TEST(type, name, op, real_part, imagine_part) \
    const type _##name   = Const(2.8, 3.1) op Variable("x");          \
    const Subtract _diff = Subtract((name), _##name);                 \
    random_complex_test({"x"}, _diff);                                \
    const Complex _complex = Complex(2.2, 1.9);                       \
    const Complex _result  = _##name.eval({{"x", _complex}});         \
    COMPARE_EPS(_result.real(), real_part);                           \
    COMPARE_EPS(_result.imag(), imagine_part);                        \
    STRING_TEST((name).str(), _##name.str(), (name), _##name)

#define CLONE_TEST(name, real_part, imagine_part)               \
    const Expression* _##name = (name).clone();                 \
    const Subtract _diff      = Subtract(name, *_##name);       \
    random_complex_test({"x"}, _diff);                          \
    const Complex _complex = Complex(2.2, 1.9);                 \
    const Complex _result  = _##name->eval({{"x", _complex}});  \
    COMPARE_EPS(_result.real(), real_part);                     \
    COMPARE_EPS(_result.imag(), imagine_part);                  \
    STRING_TEST((name).str(), _##name->str(), (name), *_##name) \
    delete _##name;

#define OPERATOR_UNARY_TEST(type, name, op, real_part, imagine_part) \
    const type _##name   = op Variable("x");                         \
    const Subtract _diff = Subtract(name, _##name);                  \
    random_complex_test({"x"}, _diff);                               \
    const Complex _complex = Complex(2.2, 1.9);                      \
    const Complex _result  = _##name.eval({{"x", _complex}});        \
    COMPARE_EPS(_result.real(), real_part);                          \
    COMPARE_EPS(_result.imag(), imagine_part)

#define UNARY_CREATION(type, name, op)                 \
    const auto name        = type(Variable("x"));      \
    const std::string _str = get_simple_unary_str(op); \
    REQUIRE(_str == (name).str())

}  // namespace

TEST_CASE("Variable test") {
    const Variable _var = Variable("x");
    SECTION("No variable in eval") {
        REQUIRE_THROWS_AS(_var.eval({}), std::out_of_range);
    }
    SECTION("Random complex") {
        Complex _complex = get_random_complex();
        REQUIRE(_var.eval({{"x", _complex}}) == _complex);
    }
}

TEST_CASE("Const test") {
    SECTION("Construct of real part") {
        const double _real     = get_rand();
        const Const _const     = Const(_real);
        const Complex _complex = _const.eval({});
        REQUIRE(_complex.real() == _real);
        REQUIRE(_complex.imag() == 0.);
    }
    SECTION("Construct of both part") {
        const double _real     = get_rand();
        const double _imagine  = get_rand();
        const Const _const     = Const(_real, _imagine);
        const Complex _complex = _const.eval({});
        REQUIRE(_complex.real() == _real);
        REQUIRE(_complex.imag() == _imagine);
    }
    SECTION("Construct of complex number") {
        const Complex _complex = get_random_complex();
        const Const _const     = Const(_complex);
        REQUIRE(_const.eval({}) == _complex);
    }
    SECTION("Clone method tests") {
        const Complex _complex   = get_random_complex();
        const Const _const       = Const(_complex);
        Expression* _cloned      = _const.clone();
        const Subtract _subtract = Subtract(_const, *_cloned);
        REQUIRE_NOTHROW(_subtract.eval({}));
        const Complex _result = _subtract.eval({});
        REQUIRE(_result.real() == 0.);
        REQUIRE(_result.imag() == 0.);
        delete _cloned;
    }
}

TEST_CASE("Add test") {
    OPERATOR_CREATION(Add, add, " + ");
    SECTION("No variable in eval") {
        REQUIRE_THROWS_AS(add.eval({}), std::out_of_range);
    }
    SECTION("Equal to operator+") {
        OPERATOR_BINARY_TEST(Add, add, +, 5., 5);
    }
    SECTION("Equal to clone") {
        CLONE_TEST(add, 5., 5.);
    }
}

TEST_CASE("Subtract test") {
    OPERATOR_CREATION(Subtract, sub, " - ");
    SECTION("No variable in eval") {
        REQUIRE_THROWS_AS(sub.eval({}), std::out_of_range);
    }
    SECTION("Equal to operator-") {
        OPERATOR_BINARY_TEST(Subtract, sub, -, 0.6, 1.2);
    }
    SECTION("Equal to clone") {
        CLONE_TEST(sub, 0.6, 1.2);
    }
}

TEST_CASE("Multiply test") {
    OPERATOR_CREATION(Multiply, mul, " * ");
    SECTION("No variable in eval") {
        REQUIRE_THROWS_AS(mul.eval({}), std::out_of_range);
    }
    SECTION("Equal to operator*") {
        OPERATOR_BINARY_TEST(Multiply, mul, *, 0.27, 12.14);
    }
    SECTION("Equal to clone") {
        CLONE_TEST(mul, 0.27, 12.14);
    }
}

TEST_CASE("Divide test") {
    OPERATOR_CREATION(Divide, div, " / ");
    SECTION("No variable in eval") {
        REQUIRE_THROWS_AS(div.eval({}), std::out_of_range);
    }
    SECTION("Equal to operator/") {
        OPERATOR_BINARY_TEST(Divide, div, /, 1.42603550295857, 0.17751479289940);
    }
    SECTION("Equal to clone") {
        CLONE_TEST(div, 1.42603550295857, 0.17751479289940);
    }
}

TEST_CASE("Negate test") {
    UNARY_CREATION(Negate, neg, "-");
    SECTION("No variable in eval") {
        REQUIRE_THROWS_AS(neg.eval({}), std::out_of_range);
    }
    SECTION("Equal to operator-") {
        OPERATOR_UNARY_TEST(Negate, neg, -, -2.2, -1.9);
    }
    SECTION("Equal to clone") {
        CLONE_TEST(neg, -2.2, -1.9);
    }
}

TEST_CASE("Conjugate test") {
    UNARY_CREATION(Conjugate, conj, "~");
    SECTION("No variable in eval") {
        REQUIRE_THROWS_AS(conj.eval({}), std::out_of_range);
    }
    SECTION("Equal to operator-") {
        OPERATOR_UNARY_TEST(Conjugate, conj, ~, 2.2, -1.9);
    }
    SECTION("Equal to clone") {
        CLONE_TEST(conj, 2.2, -1.9);
    }
}
