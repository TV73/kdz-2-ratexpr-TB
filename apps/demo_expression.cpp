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

static void showoff(const Expression& example) {
    auto success = simplify(example);
    std::cout << "до симплификации: " << example << " операций = " << countOps(example)
              << "; значение = " << example.evaluate() << "\n"
              << "после: " << *success << " операций = " << countOps(*success)
              << "; значение = " << success->evaluate() << "\n";
}

int main() {
    auto pointernumbaone = std::make_unique<Mul>(
        std::make_unique<Add>(std::make_unique<Const>(Fraction(1,2)), std::make_unique<Const>(Fraction(1,3))),
        std::make_unique<Sub>(std::make_unique<Const>(Fraction(3)), std::make_unique<Const>(Fraction(1,4))));
    std::cout << *pointernumbaone << " = " << pointernumbaone->evaluate() << " должно быть 55/24" << '\n';
    auto pointernumbatwo = std::make_unique<Negative>(
        std::make_unique<Div>(std::make_unique<Const>(Fraction(2)), std::make_unique<Const>(Fraction(3))));
    std::cout << *pointernumbatwo << " = " << pointernumbatwo->evaluate() << " должно быть -2/3" << '\n';

    std::cout << "Симплификация:"<< '\n';


    auto lot_of_additions = std::make_unique<Add>(
    std::make_unique<Add>(std::make_unique<Const>(Fraction(5)),
                        std::make_unique<Const>(Fraction(5))),
                        std::make_unique<Const>(Fraction(5)));
    showoff(*lot_of_additions);

    auto factoring_of_ecpression = std::make_unique<Add>(std::make_unique<Mul>(
        std::make_unique<Add>(std::make_unique<Const>(Fraction(1,2)), std::make_unique<Const>(Fraction(1,3))),
        std::make_unique<Sub>(std::make_unique<Const>(Fraction(3)), std::make_unique<Const>(Fraction(1,4)))),
        std::make_unique<Mul>(
        std::make_unique<Add>(std::make_unique<Const>(Fraction(1,2)), std::make_unique<Const>(Fraction(1,3))),
        std::make_unique<Const>(Fraction(42,73))));
    showoff(*factoring_of_ecpression);
    return 0;
}