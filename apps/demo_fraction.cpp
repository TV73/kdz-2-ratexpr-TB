#include <cstdint>
#include <iostream>
#include <stdexcept>
#include "ratexpr/expression.hpp"
#include <ostream>
#include <utility>
#include <memory>
#include <iostream>
#include "ratexpr/fraction.hpp"
using namespace ratexpr;

#include "ratexpr/fraction.hpp"

#include "ratexpr/simplification.hpp"
using ratexpr::Fraction;

int main() {
    Fraction half(1, 2);
    Fraction third(1, 3);
    std::cout << Fraction(6, -8) << " должно быть -3/4" << '\n';
    std::cout << Fraction(10, 4) << " должно быть 5/2" << '\n';
    std::cout << half + third << " должно быть 5/6" << '\n';
    std::cout << half - third << " должно быть 1/6" << '\n';
    std::cout << half * third << " должно быть 1/6" << '\n';
    std::cout << half / third << " должно быть 3/2" << '\n';
    std::cout << half + 2u << " должно быть 5/2" << '\n';
    std::cout << 2u + half << " должно быть 5/2" << '\n';
    std::cout << half - 2u << " должно быть -3/2" << '\n';
    std::cout << 2u - half << " должно быть 3/2" << '\n';
    std::cout << half / 2u << " должно быть 1/4" << '\n';
    std::cout << 2u / half << " должно быть 4" << '\n';
    Fraction acc;
    acc += half;
    acc += third;
    acc *= 6u;
    std::cout << acc << " должно быть 5" << '\n';
    try {
        Fraction bad(1, 0);
        std::cout << bad << '\n';
    } catch (const std::invalid_argument& e) {
        std::cout << "invalid_argument: " << e.what() << '\n';
    }
    try {
        std::cout << half / 0u << '\n';
    } catch (const std::invalid_argument& e) {
        std::cout << "invalid_argument: " << e.what() << '\n';
    }
    std::cout << Fraction(1, 2) * -1 << " holy hell is that integer overflow no way"<< '\n';
}