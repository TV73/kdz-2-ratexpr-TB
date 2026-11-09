#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include "ratexpr/expression.hpp"
#include "ratexpr/simplification.hpp"

using namespace ratexpr;
using Clock = std::chrono::steady_clock;

namespace {

double timethisshit(Clock::duration d) {
    return std::chrono::duration<double, std::milli>(d).count();
}


// Сумма 2^depth единиц
std::unique_ptr<Expression> sumofones(int depth) {
    if (depth == 0) {
        return std::make_unique<Const>(Fraction(1));
    }
    std::unique_ptr<Expression> left = sumofones(depth - 1);
    std::unique_ptr<Expression> right = sumofones(depth - 1);
    return std::make_unique<Add>(std::move(left), std::move(right));
}

// Вынести двойку за скобки
std::unique_ptr<Expression> Doublesum(int depth, std::int64_t& counter) {
    if (depth == 0) return std::make_unique<Const>(Fraction(1 + (counter++ % 5)));
    std::unique_ptr<Expression> a = Doublesum(depth - 1, counter);
    std::unique_ptr<Expression> b = Doublesum(depth - 1, counter);
    return std::make_unique<Add>(std::make_unique<Mul>(std::make_unique<Const>(Fraction(2)), std::move(a)),
                                 std::make_unique<Mul>(std::make_unique<Const>(Fraction(2)), std::move(b)));
}

// Вычитание кучи чисел - тут получается забавная структура, дающая ноль, но сокращать нечего
std::unique_ptr<Expression> Symmetry(int depth, std::int64_t& counter) {
    if (depth == 0) return std::make_unique<Const>(Fraction(2 + counter++));
    std::unique_ptr<Expression> a = Symmetry(depth - 1, counter);
    std::unique_ptr<Expression> b = Symmetry(depth - 1, counter);
    return std::make_unique<Sub>(std::move(a), std::move(b));
}

void measure(const std::string& name, const Expression& e, int reps) {
    const std::size_t opsBefore = countOps(e);
    std::size_t opsAfter = 0;
    const Fraction expected_progrev = e.evaluate();
    Fraction actual_progrev = simplify(e)->evaluate();
    double sumEval = 0;
    double sumSimplify = 0;
    double sumEvalSimple = 0;

    for (int r = 0; r < reps; ++r) {
        auto t0 = Clock::now();
        Fraction base = e.evaluate();
        auto t1 = Clock::now();
        std::unique_ptr<Expression> simple = simplify(e);
        auto t2 = Clock::now();
        Fraction evaluatedaSimple = simple->evaluate();
        auto t3 = Clock::now();
        sumEval += timethisshit(t1 - t0);
        sumSimplify += timethisshit(t2 - t1);
        sumEvalSimple += timethisshit(t3 - t2);
        opsAfter = countOps(*simple);
        if (base!=evaluatedaSimple) {
            std::cout << "Все взорвалось";
            return;
        }
    }

    const double eval = sumEval / reps;
    const double simp = sumSimplify / reps; //хехе
    const double evalSimple = sumEvalSimple / reps;
    const double total = simp + evalSimple;

    std::cout << name << "\n"
              << "Операций - " << opsBefore << "; после симплификации - " << opsAfter
              << "; значения до и после " << expected_progrev << " и " << actual_progrev << '\n'
              << "Evaluate без симплификации занимает: " << eval << " мс" << '\n'
              << "Сама симплификация занимает: " << simp << " мс" << '\n'
              << "Evaluate после симплификации занимает: " << evalSimple << " мс" << '\n'
              << "В сумме весь процесс евалуайтинга :   " << total << " мс" << '\n'
              << "По времени база -  " << eval / total << " доля от симплификации" << '\n';
}

}

int main() {
    
    const int depth = 20;   // глубина дерева
    const int reps = 5;     // число повторов
    std::cout << "глубина " << depth << ", повторов " << reps << "\n\n";
    std::unique_ptr<Expression> e = sumofones(depth);
    measure("Больше единиц богу единиц", *e, reps);
    std::int64_t counter_2 = 0;
    std::unique_ptr<Expression> ee = Doublesum(depth - 2, counter_2);
    measure("Вынос двоечки", *ee, reps);
    std::int64_t counter_3 = 0;
    std::unique_ptr<Expression> eee = Symmetry(depth, counter_3);
    measure("Несокращаемая симметрия", *eee, reps);
    return 0;
}