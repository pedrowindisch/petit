#pragma once

#include "ast/expr.hpp"

class NumberExpr : public Expr {
    public:
        explicit NumberExpr(double value) : value(value) {}

        double getValue() const {
            return value;
        }

    private:
        double value;
};
