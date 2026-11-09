#pragma once
#include <cstddef>
#include <memory>

#include "ratexpr/expression.hpp"

namespace ratexpr {

std::unique_ptr<Expression> simplify(const Expression& e); //ну тут все очев

std::size_t countOps(const Expression& e); //Подсчет операций АКА насколько мы умом

}