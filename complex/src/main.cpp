#include <iostream>

#include "Complex.hpp"
#include "Expressions.hpp"

int main() {
    const Add expr(Multiply(Const(2), Variable("x")), Const(1));
    const Complex result = expr.eval({{"x", Complex(100.)}});
    std::cout << "Expression = " << expr << std::endl;
    std::cout << "Result = " << result << std::endl;
}
