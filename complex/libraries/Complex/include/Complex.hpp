#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include <cmath>
#include <ostream>
#include <string>

class Complex {
private:
    double _real;
    double _imagine;

public:
    explicit Complex(double real = 0.) noexcept;
    Complex(double real, double imagine) noexcept;
    Complex(const Complex& other) noexcept;
    Complex(Complex&& other) noexcept;

    [[nodiscard]] double multiply_by_conjugate() const noexcept;
    [[nodiscard]] double real() const noexcept;
    [[nodiscard]] double imag() const noexcept;
    [[nodiscard]] double abs() const noexcept;
    [[nodiscard]] std::string str() const noexcept;
    Complex operator+(const Complex& other) const noexcept;
    Complex& operator+=(const Complex& other) noexcept;
    Complex operator-(const Complex& other) const noexcept;
    Complex& operator-=(const Complex& other) noexcept;
    Complex operator*(const Complex& other) const noexcept;
    Complex& operator*=(const Complex& other) noexcept;
    Complex operator/(const Complex& other) const noexcept;
    Complex& operator/=(const Complex& other) noexcept;
    Complex operator*(double value) const noexcept;
    Complex& operator*=(double value) noexcept;
    Complex operator/(double value) const noexcept;
    Complex& operator/=(double value) noexcept;

    Complex& operator=(const Complex& other) noexcept;
    Complex& operator=(Complex&& other) noexcept;

    friend Complex operator*(double value, const Complex& c) noexcept { return Complex(c) *= value; }
    friend Complex operator/(double value, const Complex& c) noexcept { return Complex(value) /= c; }

    Complex operator-() const noexcept;
    Complex operator~() const noexcept;

    bool operator==(const Complex& other) const;
    bool operator!=(const Complex& other) const;

    friend std::ostream& operator<<(std::ostream& out, const Complex& c) {
        out << c.str();
        return out;
    }
};

#endif
