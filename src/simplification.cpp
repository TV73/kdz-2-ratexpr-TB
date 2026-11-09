#include "ratexpr/simplification.hpp"

#include <sstream>
#include <utility>

namespace ratexpr {

namespace {

using Ptr = std::unique_ptr<Expression>;

bool same(const Expression& x, const Expression& y) {
    std::ostringstream sx;
    std::ostringstream sy;
    sx << x;
    sy << y;
    return sx.str() == sy.str();
} // fancy way of checking

// вынос константы
const Expression& splitCoefficient(const Expression& e, Fraction& k) {
    const Mul* m = dynamic_cast<const Mul*>(&e);
    if (m != nullptr) { //умножение?
        const Const* right = dynamic_cast<const Const*>(&m->second()); //наличие константц
        if (right != nullptr) {
            k = right->value();
            return m->first();
        }
        const Const* left = dynamic_cast<const Const*>(&m->first()); //наличие константц
        if (left != nullptr) {
            k = left->value();
            return m->second();
        }
    }
    k = Fraction(1);
    return e;
}

// Its not the scale its how u use it
Ptr scaled(const Expression& base, const Fraction& k) {
    if (k == Fraction(0)) {
        return std::make_unique<Const>(Fraction(0));
    }
    if (k == Fraction(1)) {
        return base.clone();
    }
    return std::make_unique<Mul>(base.clone(), std::make_unique<Const>(k));
}

Ptr makeMul(Ptr a, Ptr c) {
    const Const* const_a_check = dynamic_cast<const Const*>(a.get());
    const Const* const_c_check = dynamic_cast<const Const*>(c.get());
    if (const_a_check != nullptr && const_a_check->value() == Fraction(1)) {
        return c;
    }
    if (const_c_check != nullptr && const_c_check->value() == Fraction(1)) {
        return a;
    }
    return std::make_unique<Mul>(std::move(a), std::move(c));
} //Убирает x*1 & 1*x (топ ГЕНИАЛЬНЫХ симплификаций)

Ptr makeDiv(Ptr a, Ptr c) {
    const Const* const_c_check = dynamic_cast<const Const*>(c.get());
    if (const_c_check != nullptr && const_c_check->value() == Fraction(1)) {
        return a;
    }
    return std::make_unique<Div>(std::move(a), std::move(c));
} //Same shi but for division

Ptr makeSum(Ptr a, Ptr c, bool isAdd); //обьявляю, чтобы юзать в tryfactor

// a*b + a*c = a*(b+c) & a*b - a*c = a*(b-c)
Ptr tryFactor(const Expression& a, const Expression& c, bool isAdd) {
    const Mul* m = dynamic_cast<const Mul*>(&a);
    const Mul* n = dynamic_cast<const Mul*>(&c);
    if (m == nullptr || n == nullptr) return nullptr;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            const Expression& common1 = (i == 0) ? m->first() : m->second();
            const Expression& rest1 = (i == 0) ? m->second() : m->first();
            const Expression& common2 = (j == 0) ? n->first() : n->second();
            const Expression& rest2 = (j == 0) ? n->second() : n->first();
            //fancy перебор четырех вариантов
            if (same(common1, common2)) {
                return makeMul(common1.clone(),
                               makeSum(rest1.clone(), rest2.clone(), isAdd));
            }
        }
    }
    return nullptr; //ничего не вышло:(
}

Ptr makeSum(Ptr a, Ptr c, bool isAdd) {
    const Const* const_a_check = dynamic_cast<const Const*>(a.get());
    const Const* const_c_check = dynamic_cast<const Const*>(c.get());
    if (const_a_check != nullptr && const_a_check->value() == Fraction(0) && isAdd) {
        return c;
    }
    if (const_c_check != nullptr && const_c_check->value() == Fraction(0)) {
        return a;
    } //ГЕНИАЛЬНЕЙШИЕ симплификации

    Fraction ka;
    Fraction kc;
    const Expression& baseA = splitCoefficient(*a, ka);
    const Expression& baseC = splitCoefficient(*c, kc);
    if (same(baseA, baseC)) {// x*c_1 + x*c_2 = x*(c_1 + c_2)
        if (isAdd) {
            return scaled(baseA, ka + kc);
        }
        else {
            return scaled(baseA, ka - kc);
        }
    }

    Ptr factored = tryFactor(*a, *c, isAdd);// a*b + a*c = a*(b + c)
    if (factored) {
        return factored;
    }

    if (isAdd) {
        return std::make_unique<Add>(std::move(a), std::move(c));
    }
    else {
        return std::make_unique<Sub>(std::move(a), std::move(c));
    }
}

}

std::size_t countOps(const Expression& e) {
    if (const Negative* n = dynamic_cast<const Negative*>(&e)) {
        return 1 + countOps(n->operand());
    }
    if (const BinaryExpression* b = dynamic_cast<const BinaryExpression*>(&e)) {
        return 1 + countOps(b->first()) + countOps(b->second());
    }
    return 0;
    //Рекурсивная штука - если унарный минус то go deeper + 1
    //Если нная операция то посчитаем сколько штук внутри в обоих + 1
    //Если конст - то 0
}

std::unique_ptr<Expression> simplify(const Expression& e) {
    if (const Negative* n = dynamic_cast<const Negative*>(&e)) {
        return std::make_unique<Negative>(simplify(n->operand()));
    }
    const BinaryExpression* b = dynamic_cast<const BinaryExpression*>(&e);
    if (b == nullptr) {
        return e.clone();
    }
    Ptr a = simplify(b->first());
    Ptr c = simplify(b->second());
    if (dynamic_cast<const Add*>(&e)) return makeSum(std::move(a), std::move(c), true);
    if (dynamic_cast<const Sub*>(&e)) return makeSum(std::move(a), std::move(c), false);
    if (dynamic_cast<const Mul*>(&e)) return makeMul(std::move(a), std::move(c));
    return makeDiv(std::move(a), std::move(c));
}

}