// eval.cpp
#include <stdexcept>
#include <cmath>
#include "eval.hpp"
#include "env/env.hpp"
#include "ast/expr.hpp"
#include "ast/number_expr.hpp"
#include "ast/symbol_expr.hpp"
#include "ast/list_expr.hpp"
#include <iostream>

Expr* eval(Expr* expr, Env& env) {
    if (auto* number = dynamic_cast<NumberExpr*>(expr)) return expr;
    if (auto* symbol = dynamic_cast<SymbolExpr*>(expr)) return nullptr;
    if (auto* list = dynamic_cast<ListExpr*>(expr)) {
        if (list->size() == 0) throw std::runtime_error("Cannot evaluate empty list");

        auto head = list->get(0);
        auto* op = dynamic_cast<SymbolExpr*>(head);
        if (!op) throw std::runtime_error("First element must be an operator symbol");

        if (op->getSymbol() == "+") {
            double result = 0;
            for (size_t i = 1; i < list->size(); ++i) {
                auto val = eval(list->get(i), env);
                auto* num = dynamic_cast<NumberExpr*>(val);
                if (!num) throw std::runtime_error("Expected number in '+'");
                result += num->getValue();
            }

            return new NumberExpr(result);
        }

        if (op->getSymbol() == "-") {
            if (list->size() != 3) throw std::runtime_error("Subtraction requires exactly two operands");
            auto* left = eval(list->get(1), env);
            auto* right = eval(list->get(2), env);
            auto* leftNum = dynamic_cast<NumberExpr*>(left);
            auto* rightNum = dynamic_cast<NumberExpr*>(right);
            if (!leftNum || !rightNum) throw std::runtime_error("Expected numbers in '-'");

            return new NumberExpr(leftNum->getValue() - rightNum->getValue());
        }

        if (op->getSymbol() == "*") {
            double result = 1;
            for (size_t i = 1; i < list->size(); ++i) {
                auto val = eval(list->get(i), env);
                auto* num = dynamic_cast<NumberExpr*>(val);
                if (!num) throw std::runtime_error("Expected number in '*'");
                result *= num->getValue();
            }

            return new NumberExpr(result);
        }

        if (op->getSymbol() == "/") {
            if (list->size() != 3) throw std::runtime_error("Division requires exactly two operands");
            auto* left = eval(list->get(1), env);
            auto* right = eval(list->get(2), env);
            auto* leftNum = dynamic_cast<NumberExpr*>(left);
            auto* rightNum = dynamic_cast<NumberExpr*>(right);
            if (!leftNum || !rightNum) throw std::runtime_error("Expected numbers in '/'");
            if (rightNum->getValue() == 0) throw std::runtime_error("Division by zero");
            
            return new NumberExpr(leftNum->getValue() / rightNum->getValue());
        }
    }
}
