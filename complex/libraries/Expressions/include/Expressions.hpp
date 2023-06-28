#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <map>
#include <ostream>

#include "Complex.hpp"

class Add;
class Subtract;
class Multiply;
class Divide;
class Negate;
class Conjugate;

class Expression {
public:
    virtual Complex eval(const std::map<std::string, Complex>& variables) const = 0;
    [[nodiscard]] virtual Expression* clone() const                             = 0;

    friend Add operator+(const Expression& lhs, const Expression& rhs);
    //    Add operator+(const Expression& rhs) const;
    Subtract operator-(const Expression& rhs) const;
    Multiply operator*(const Expression& rhs) const;
    Divide operator/(const Expression& rhs) const;
    Negate operator-() const;
    Conjugate operator~() const;

    virtual std::string str() const = 0;

    friend std::ostream& operator<<(std::ostream& out, const Expression& expr);

    virtual ~Expression() = default;
};

class Const: public Expression {
private:
    const Complex _value;

public:
    explicit Const(double real = 0., double imagine = 0.) noexcept;
    explicit Const(const Complex& other) noexcept;

    Complex eval(const std::map<std::string, Complex>&) const override;
    [[nodiscard]] Expression* clone() const override;

    std::string str() const override;
};

class Variable: public Expression {
private:
    const std::string _name;

public:
    explicit Variable(const std::string& name) noexcept;
    explicit Variable(std::string&& name) noexcept;

    Complex eval(const std::map<std::string, Complex>& variables) const override;
    [[nodiscard]] Expression* clone() const override;

    std::string str() const override;
};

class Unary: public Expression {
protected:
    const std::string _op;

    const Expression* _expression;

    Unary(std::string&& op, const Unary& other);
    explicit Unary(std::string&& op, const Expression* expression) noexcept;

public:
    std::string str() const override;

    ~Unary() override;
};

class Negate: public Unary {
protected:
    Negate(const Negate& other);

public:
    explicit Negate(const Expression& expression);

    Complex eval(const std::map<std::string, Complex>& variables) const override;

    [[nodiscard]] Expression* clone() const override;
};

class Conjugate: public Unary {
protected:
    Conjugate(const Conjugate& other);

public:
    explicit Conjugate(const Expression& expression);

    Complex eval(const std::map<std::string, Complex>& variables) const override;

    [[nodiscard]] Expression* clone() const override;
};

class Binary: public Expression {
protected:
    const std::string _op;

    const Expression* _left;
    const Expression* _right;

    Binary(std::string&& op, const Binary& other);
    Binary(std::string&& op, const Expression* left, const Expression* right);

public:
    std::string str() const override;

    ~Binary() override;
};

class Add: public Binary {
protected:
    Add(const Add& other);

public:
    Add(const Expression& left, const Expression& right);

    Complex eval(const std::map<std::string, Complex>& variables) const override;

    [[nodiscard]] Expression* clone() const override;
};

class Subtract: public Binary {
protected:
    Subtract(const Subtract& other);

public:
    Subtract(const Expression& left, const Expression& right);

    Complex eval(const std::map<std::string, Complex>& variables) const override;

    [[nodiscard]] Expression* clone() const override;
};

class Multiply: public Binary {
protected:
    Multiply(const Multiply& other);

public:
    Multiply(const Expression& left, const Expression& right);

    Complex eval(const std::map<std::string, Complex>& variables) const override;

    [[nodiscard]] Expression* clone() const override;
};

class Divide: public Binary {
protected:
    Divide(const Divide& other);

public:
    Divide(const Expression& left, const Expression& right);

    Complex eval(const std::map<std::string, Complex>& variables) const override;

    [[nodiscard]] Expression* clone() const override;
};

#endif
