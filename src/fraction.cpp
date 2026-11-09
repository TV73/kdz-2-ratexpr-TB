#include "ratexpr/fraction.hpp"

#include <ostream>
#include <stdexcept>

namespace ratexpr {

Fraction::Fraction() : numer_(0), den_(1) {}

Fraction::Fraction(std::int64_t numer) : numer_(numer), den_(1) {}

Fraction::Fraction(std::int64_t num, std::int64_t den) : numer_(0), den_(1) {
    Assign(num, den);
}

Fraction& Fraction::operator+=(const Fraction& other) {
    long long n = static_cast<long long>(numer_) * other.den_ + static_cast<long long>(other.numer_) * den_;
    long long d = static_cast<long long>(den_) * other.den_;
    Assign(n, d);
    return *this;
}

Fraction& Fraction::operator+=(std::uint32_t other) {
    long long n = static_cast<long long>(numer_) + static_cast<long long>(other) * den_;
    long long d = static_cast<long long>(den_);
    Assign(n, d);
    return *this;
}

Fraction& Fraction::operator-=(const Fraction& other) {
    long long n = static_cast<long long>(numer_) * other.den_ - static_cast<long long>(other.numer_) * den_;
    long long d = static_cast<long long>(den_) * other.den_;
    Assign(n, d);
    return *this;
}

Fraction& Fraction::operator-=(std::uint32_t other) {
    long long n = static_cast<long long>(numer_) - static_cast<long long>(other) * den_;
    long long d = static_cast<long long>(den_);
    Assign(n, d);
    return *this;
}

Fraction& Fraction::operator*=(const Fraction& other) {
    long long n = static_cast<long long>(numer_) * other.numer_;
    long long d = static_cast<long long>(den_) * other.den_;
    Assign(n, d);
    return *this;
}

Fraction& Fraction::operator*=(std::uint32_t other) {
    long long n = static_cast<long long>(numer_) * other;
    long long d = static_cast<long long>(den_);
    Assign(n, d);
    return *this;
}

Fraction& Fraction::operator/=(const Fraction& other) {
    long long n = static_cast<long long>(numer_) * other.den_;
    long long d = static_cast<long long>(den_) * other.numer_;
    if (d == 0) {
        throw std::invalid_argument("деление на ноль!");
    }
    Assign(n, d);
    return *this;
}

Fraction& Fraction::operator/=(std::uint32_t other) {
    long long n = static_cast<long long>(numer_);
    if (other == 0) {
        throw std::invalid_argument("деление на ноль!");
    }
    long long d = static_cast<long long>(den_) * other;
    Assign(n, d);
    return *this;
}

Fraction Fraction::operator+() const {
    return *this;
}

Fraction Fraction::operator-() const {
    return Fraction(-numer_, den_);
}

Fraction& Fraction::operator++() {
    numer_ += den_;
    return *this;
}

Fraction& Fraction::operator--() {
    numer_ -= den_;
    return *this;
}

long long Fraction::Gcd(long long a, long long b) {
    while (b != 0) {
        long long rem_eucl = a % b;
        a = b;
        b = rem_eucl;
    }
    return a;
}

void Fraction::Assign(long long n, long long d) {
    if (d < 0) {
        n = -n;
        d = -d;
    }
    if (d == 0) {
        throw std::invalid_argument("деление на ноль!");
    }
    if (n == 0) {
        numer_ = 0;
        den_ = 1;
        return;
    }
    long long gcd_temp = 0;
    if (n > 0) {
        gcd_temp = Gcd(n, d);
    } else {
        gcd_temp = Gcd(-n, d);
    }

    numer_ = static_cast<std::int64_t>(n / gcd_temp);
    den_ = static_cast<std::int64_t>(d / gcd_temp);
}

Fraction operator+(const Fraction& a, const Fraction& b) {
    Fraction result_add = a;
    result_add += b;
    return result_add;
}

Fraction operator+(std::uint32_t a, const Fraction& b) {
    Fraction result_add = b;
    result_add += a;
    return result_add;
}

Fraction operator+(const Fraction& a, std::uint32_t b) {
    Fraction result_add = a;
    result_add += b;
    return result_add;
}

Fraction operator-(const Fraction& a, const Fraction& b) {
    Fraction result_sun = a;
    result_sun -= b;
    return result_sun;
}

Fraction operator-(std::uint32_t a, const Fraction& b) {
    Fraction result_sun = -b;
    result_sun += a;
    return result_sun;
}

Fraction operator-(const Fraction& a, std::uint32_t b) {
    Fraction result_sun = a;
    result_sun -= b;
    return result_sun;
}

Fraction operator*(const Fraction& a, std::uint32_t b) {
    Fraction result_mul = a;
    result_mul *= b;
    return result_mul;
}

Fraction operator*(std::uint32_t a, const Fraction& b) {
    Fraction result_mul = b;
    result_mul *= a;
    return result_mul;
}

Fraction operator*(const Fraction& a, const Fraction& b) {
    Fraction result_mul = a;
    result_mul *= b;
    return result_mul;
}

Fraction operator/(const Fraction& a, const Fraction& b) {
    Fraction result_div = a;
    result_div /= b;
    return result_div;
}

Fraction operator/(std::uint32_t a, const Fraction& b) {
    Fraction result_div(a, 1);
    result_div /= b;
    return result_div;
}

Fraction operator/(const Fraction& a, std::uint32_t b) {
    Fraction division_stuff(1, b);
    Fraction result_div = a;
    result_div *= division_stuff;
    return result_div;
}

std::ostream& operator<<(std::ostream& os, const Fraction& output) {
    if (output.den_ == 1) {
        os << output.numer_;
    } else {
        os << output.numer_ << '/' << output.den_;
    }
    return os;
}

bool operator==(const Fraction& a, const Fraction& b) {
    return a.Numerator() == b.Numerator() && a.Denominator() == b.Denominator();
}

bool operator!=(const Fraction& a, const Fraction& b) {
    return !(a == b);
}

}