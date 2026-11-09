#include "ratexpr/expression.hpp"
#include <ostream>
#include <utility>
namespace ratexpr {

std::ostream& operator<<(std::ostream& os, const Expression& smth) {
    smth.print(os);
    return os;
}

Const::Const(const Fraction& value): value_(value) {

}

Fraction Const::evaluate() const {
    return value_;

}

void Const:: print(std::ostream& os) const {
    os << value_;
}

Negative::Negative(std::unique_ptr<Expression> operand): operand_(std::move(operand)) {
}

Fraction Negative::evaluate() const {
    return -operand_->evaluate();

}

void Negative:: print(std::ostream& os) const {
    os << "(-" << *operand_ << ")";
}

BinaryExpression:: BinaryExpression(std::unique_ptr<Expression> first,
    std::unique_ptr<Expression> second): first_(std::move(first)), second_(std::move(second)) {

} 



Add::Add(std::unique_ptr<Expression> first, std::unique_ptr<Expression> second): BinaryExpression(std::move(first), std::move(second)) {

}

Sub::Sub(std::unique_ptr<Expression> first, std::unique_ptr<Expression> second): BinaryExpression(std::move(first), std::move(second)) {

}

Mul::Mul(std::unique_ptr<Expression> first, std::unique_ptr<Expression> second): BinaryExpression(std::move(first), std::move(second)) {

}

Div::Div(std::unique_ptr<Expression> first, std::unique_ptr<Expression> second): BinaryExpression(std::move(first), std::move(second)) {

}

Fraction Add::evaluate() const {
    return first_->evaluate()+second_->evaluate();
}
Fraction Sub::evaluate() const {
    return first_->evaluate()-second_->evaluate();
}
Fraction Mul::evaluate() const {
    return first_->evaluate()*second_->evaluate();
}
Fraction Div::evaluate() const {
    return first_->evaluate()/second_->evaluate();
}

void Add:: print(std::ostream& os) const {
    os << "(" << *first_ << " + " << *second_ << ")";
}

void Sub:: print(std::ostream& os) const {
    os << "(" << *first_ << " - " << *second_ << ")";
}

void Mul:: print(std::ostream& os) const {
    os << "(" << *first_ << " * " << *second_ << ")";
}

void Div:: print(std::ostream& os) const {
    os << "(" << *first_ << " / " << *second_ << ")";
}

//something terrible happened here... (костыли)

std::unique_ptr<Expression> Const::clone() const {
    return std::make_unique<Const>(value_);
}
std::unique_ptr<Expression> Negative::clone() const {
    return std::make_unique<Negative>(operand_->clone());
}
std::unique_ptr<Expression> Add::clone() const {
    return std::make_unique<Add>(first_->clone(), second_->clone());
}
std::unique_ptr<Expression> Sub::clone() const {
    return std::make_unique<Sub>(first_->clone(), second_->clone());
}
std::unique_ptr<Expression> Mul::clone() const {
    return std::make_unique<Mul>(first_->clone(), second_->clone());
}
std::unique_ptr<Expression> Div::clone() const {
    return std::make_unique<Div>(first_->clone(), second_->clone());
}
}