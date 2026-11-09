#pragma once
#include <iosfwd>
#include <memory>

#include "ratexpr/fraction.hpp"

namespace ratexpr {

class Expression {
public:
    virtual ~Expression() = default;
    virtual Fraction evaluate() const = 0;
    virtual void print(std::ostream& os) const = 0;

    virtual std::unique_ptr<Expression> clone() const = 0; //Для симплификейшна
};

std::ostream& operator<<(std::ostream& os, const Expression& e);

class Const : public Expression {
public:
    explicit Const(const Fraction& value);
    Fraction evaluate() const override;
    void print(std::ostream& os) const override;

    std::unique_ptr<Expression> clone() const override; //Для симплификейшна
    const Fraction& value() const {
        return value_;
    }
private:
    Fraction value_;
};

class Negative : public Expression {
public:
    explicit Negative(std::unique_ptr<Expression> operand);
    Fraction evaluate() const override;
    void print(std::ostream& os) const override;

    std::unique_ptr<Expression> clone() const override; //Для симплификейшна
    const Expression& operand() const {
        return *operand_;
    }
private:
    std::unique_ptr<Expression> operand_;
};

class BinaryExpression : public Expression {
public:
    BinaryExpression(std::unique_ptr<Expression> first,
                     std::unique_ptr<Expression> second);

    const Expression& first() const {  //Для симплификейшна
        return *first_;
    }
    const Expression& second() const {
        return *second_;
    }

protected:
    std::unique_ptr<Expression> first_;
    std::unique_ptr<Expression> second_;
};

class Add : public BinaryExpression {
public:
    Add(std::unique_ptr<Expression> first, std::unique_ptr<Expression> second);
    Fraction evaluate() const override;
    void print(std::ostream& os) const override;

    std::unique_ptr<Expression> clone() const override;   //Для симплификейшна
};

class Sub : public BinaryExpression {
public:
    Sub(std::unique_ptr<Expression> first, std::unique_ptr<Expression> second);
    Fraction evaluate() const override;
    void print(std::ostream& os) const override;

    std::unique_ptr<Expression> clone() const override;  //Для симплификейшна
};

class Mul : public BinaryExpression {
public:
    Mul(std::unique_ptr<Expression> first, std::unique_ptr<Expression> second);
    Fraction evaluate() const override;
    void print(std::ostream& os) const override;

    std::unique_ptr<Expression> clone() const override;  //Для симплификейшна
};

class Div : public BinaryExpression {
public:
    Div(std::unique_ptr<Expression> first, std::unique_ptr<Expression> second);
    Fraction evaluate() const override;
    void print(std::ostream& os) const override;

    std::unique_ptr<Expression> clone() const override;  //Для симплификейшна
};

}