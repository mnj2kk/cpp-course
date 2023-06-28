#include "Expressions.hpp"

// Add Expression::operator+(const Expression& rhs) const

Subtract Expression::operator-(const Expression& rhs) const {
    return {*this, rhs};
}

Multiply Expression::operator*(const Expression& rhs) const {
    return {*this, rhs};
}

Divide Expression::operator/(const Expression& rhs) const {
    return {*this, rhs};
}

Negate Expression::operator-() const {
    return Negate(*this);
}

Conjugate Expression::operator~() const {
    return Conjugate(*this);
}

std::ostream& operator<<(std::ostream& out, const Expression& expr) {
    return out << expr.str();
}
Add operator+(const Expression& lhs, const Expression& rhs) {
    return {lhs, rhs};
}

Const::Const(double real, double imagine) noexcept : _value{real, imagine} {}

Const::Const(const Complex& other) noexcept : _value(other) {}

Complex Const::eval(const std::map<std::string, Complex>&) const {
    return _value;
}

Expression* Const::clone() const {
    return new Const(*this);
}

std::string Const::str() const {
    return "(" + _value.str() + ")";
}

Variable::Variable(const std::string& name) noexcept : _name(name) {}

Variable::Variable(std::string&& name) noexcept : _name(std::forward<std::string>(name)) {}

Complex Variable::eval(const std::map<std::string, Complex>& variables) const {
    return variables.at(_name);  // throw std::out_of_range
}

Expression* Variable::clone() const {
    return new Variable(*this);
}

std::string Variable::str() const {
    return _name;
}

Unary::Unary(std::string&& op, const Unary& other)
    : _op(std::forward<std::string>(op)), _expression(other._expression->clone()) {}

Unary::Unary(std::string&& op, const Expression* expression) noexcept
    : _op(std::forward<std::string>(op)), _expression(expression) {}

Unary::~Unary() {
    delete _expression;
}
std::string Unary::str() const {
    return _op + "(" + _expression->str() + ")";
}

Negate::Negate(const Negate& other) : Unary("-", other._expression->clone()) {}

Negate::Negate(const Expression& expression) : Unary("-", expression.clone()) {}

Complex Negate::eval(const std::map<std::string, Complex>& variables) const {
    return -_expression->eval(variables);
}

Expression* Negate::clone() const {
    return new Negate(*this);
}

Conjugate::Conjugate(const Conjugate& other) : Unary("~", other._expression->clone()) {}

Conjugate::Conjugate(const Expression& expression) : Unary("~", expression.clone()) {}

Complex Conjugate::eval(const std::map<std::string, Complex>& variables) const {
    return ~_expression->eval(variables);
}

Expression* Conjugate::clone() const {
    return new Conjugate(*this);
}

Binary::Binary(std::string&& op, const Binary& other)
    : _op(std::forward<std::string>(op)), _left(other._left->clone()), _right(other._right->clone()) {}

Binary::Binary(std::string&& op, const Expression* left, const Expression* right)
    : _op(std::forward<std::string>(op)), _left(left), _right(right) {}

std::string Binary::str() const {
    return "(" + _left->str() + _op + _right->str() + ")";
}

Binary::~Binary() {
    delete _left;
    delete _right;
}

Add::Add(const Add& other) : Binary(" + ", other._left->clone(), other._right->clone()) {}

Add::Add(const Expression& left, const Expression& right) : Binary(" + ", left.clone(), right.clone()) {}

Complex Add::eval(const std::map<std::string, Complex>& variables) const {
    return _left->eval(variables) + _right->eval(variables);
}

Expression* Add::clone() const {
    return new Add(*this);
}

Subtract::Subtract(const Subtract& other) : Binary(" - ", other._left->clone(), other._right->clone()) {}

Subtract::Subtract(const Expression& left, const Expression& right) : Binary(" - ", left.clone(), right.clone()) {}

Complex Subtract::eval(const std::map<std::string, Complex>& variables) const {
    return _left->eval(variables) - _right->eval(variables);
}

Expression* Subtract::clone() const {
    return new Subtract(*this);
}

Multiply::Multiply(const Multiply& other) : Binary(" * ", other._left->clone(), other._right->clone()) {}

Multiply::Multiply(const Expression& left, const Expression& right) : Binary(" * ", left.clone(), right.clone()) {}

Complex Multiply::eval(const std::map<std::string, Complex>& variables) const {
    return _left->eval(variables) * _right->eval(variables);
}

Expression* Multiply::clone() const {
    return new Multiply(*this);
}

Divide::Divide(const Divide& other) : Binary(" / ", other._left->clone(), other._right->clone()) {}

Divide::Divide(const Expression& left, const Expression& right) : Binary(" / ", left.clone(), right.clone()) {}

Complex Divide::eval(const std::map<std::string, Complex>& variables) const {
    return _left->eval(variables) / _right->eval(variables);
}

Expression* Divide::clone() const {
    return new Divide(*this);
}
