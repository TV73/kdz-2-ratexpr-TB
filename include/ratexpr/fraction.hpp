#pragma once
#include <cstdint>
#include <iosfwd>

namespace ratexpr {

class Fraction {
public:
    Fraction();
    Fraction(std::int64_t numer);
    Fraction(std::int64_t num, std::int64_t den);

    std::int64_t Numerator() const { return numer_; }
    std::int64_t Denominator() const { return den_; }

    Fraction& operator+=(const Fraction& other);
    Fraction& operator+=(std::uint32_t other);
    Fraction& operator-=(const Fraction& other);
    Fraction& operator-=(std::uint32_t other);
    Fraction& operator*=(const Fraction& other);
    Fraction& operator*=(std::uint32_t other);
    Fraction& operator/=(const Fraction& other);
    Fraction& operator/=(std::uint32_t other);

    Fraction operator+() const;
    Fraction operator-() const;
    Fraction& operator++();
    Fraction& operator--();

    friend Fraction operator+(const Fraction& a, const Fraction& b);
    friend Fraction operator+(const Fraction& a, std::uint32_t b);
    friend Fraction operator+(std::uint32_t a, const Fraction& b);

    friend Fraction operator-(const Fraction& a, const Fraction& b);
    friend Fraction operator-(const Fraction& a, std::uint32_t b);
    friend Fraction operator-(std::uint32_t a, const Fraction& b);

    friend Fraction operator*(const Fraction& a, const Fraction& b);
    friend Fraction operator*(const Fraction& a, std::uint32_t b);
    friend Fraction operator*(std::uint32_t a, const Fraction& b);

    friend Fraction operator/(const Fraction& a, const Fraction& b);
    friend Fraction operator/(const Fraction& a, std::uint32_t b);
    friend Fraction operator/(std::uint32_t a, const Fraction& b);

    friend std::ostream& operator<<(std::ostream& os, const Fraction& output);

private:
    static long long Gcd(long long a, long long b);
    void Assign(long long n, long long d);

    std::int64_t numer_;
    std::int64_t den_;
};

bool operator==(const Fraction& a, const Fraction& b);
bool operator!=(const Fraction& a, const Fraction& b);

}