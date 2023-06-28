#include "Complex.hpp"

Complex::Complex(double real) noexcept : _real(real), _imagine() {}

Complex::Complex(double real, double imagine) noexcept : _real(real), _imagine(imagine) {}

Complex::Complex(const Complex &other) noexcept = default;

Complex::Complex(Complex &&other) noexcept : _real(other.real()), _imagine(other._imagine) {}

double Complex::multiply_by_conjugate() const noexcept {
    return _real * _real + _imagine * _imagine;
}

double Complex::real() const noexcept {
    return _real;
}

double Complex::imag() const noexcept {
    return _imagine;
}

double Complex::abs() const noexcept {
    return std::sqrt(multiply_by_conjugate());
}

std::string Complex::str() const noexcept {
    return std::to_string(_real) + " + " + std::to_string(_imagine) + " * i";
}

Complex Complex::operator+(const Complex &other) const noexcept {
    return Complex(*this) += other;
}

Complex &Complex::operator+=(const Complex &other) noexcept {
    _real += other._real;
    _imagine += other._imagine;
    return *this;
}

Complex Complex::operator-(const Complex &other) const noexcept {
    return Complex(*this) -= other;
}

Complex &Complex::operator-=(const Complex &other) noexcept {
    _real -= other._real;
    _imagine -= other._imagine;
    return *this;
}

Complex Complex::operator*(const Complex &other) const noexcept {
    return Complex(*this) *= other;
}

Complex &Complex::operator*=(const Complex &other) noexcept {
    const double old_real    = _real;
    const double old_imagine = _imagine;
    if (&other == this) {
        _real    = old_real * old_real - old_imagine * old_imagine;
        _imagine = 2. * old_imagine * old_real;
    } else {
        _real    = old_real * other._real - old_imagine * other._imagine;
        _imagine = old_real * other._imagine + old_imagine * other._real;
    }
    return *this;
}

Complex Complex::operator/(const Complex &other) const noexcept {
    return Complex(*this) /= other;
}

Complex &Complex::operator/=(const Complex &other) noexcept {
    Complex conjugate = ~other;
    return this->operator/=(other.multiply_by_conjugate()).operator*=(conjugate);
}

Complex Complex::operator*(double value) const noexcept {
    return Complex(*this) *= value;
}

Complex &Complex::operator*=(double value) noexcept {
    _real *= value;
    _imagine *= value;
    return *this;
}

Complex Complex::operator/(double value) const noexcept {
    return Complex(*this) /= value;
}

Complex &Complex::operator/=(double value) noexcept {
    _real /= value;
    _imagine /= value;
    return *this;
}

Complex &Complex::operator=(const Complex &other) noexcept {
    if (this != &other) {
        _real    = other._real;
        _imagine = other._imagine;
    }
    return *this;
}

Complex &Complex::operator=(Complex &&other) noexcept {
    if (this != &other) {
        _real    = other._real;
        _imagine = other._imagine;
    }
    return *this;
}

Complex Complex::operator-() const noexcept {
    return {-_real, -_imagine};
}

Complex Complex::operator~() const noexcept {
    return {_real, -_imagine};
}

bool Complex::operator==(const Complex &other) const {
    return _real == other._real && _imagine == other._imagine;
}

bool Complex::operator!=(const Complex &other) const {
    return !this->operator==(other);
}
