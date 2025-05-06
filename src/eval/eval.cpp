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

NumberExpr* evalMathOp(const std::string& op, NumberExpr* left, NumberExpr* right) {
    if (!left || !right) throw std::runtime_error("Invalid operands for math operation");
    
    if (op == "+") return new NumberExpr(left->getValue() + right->getValue());
    if (op == "-") return new NumberExpr(left->getValue() - right->getValue());
    if (op == "*") return new NumberExpr(left->getValue() * right->getValue());
    if (op == "/") {
        if (right->getValue() == 0) throw std::runtime_error("Division by zero");
        return new NumberExpr(left->getValue() / right->getValue());
    }
    throw std::runtime_error("Unknown operator: " + op);
}

NumberExpr* evalBoolOp(const std::string& op, NumberExpr* left, NumberExpr* right) {
    if (!left || !right) throw std::runtime_error("Invalid operands for comparison operation");

    if (op == ">") return left->getValue() > right->getValue() ? new NumberExpr(1) : new NumberExpr(0);
    if (op == ">=") return left->getValue() >= right->getValue() ? new NumberExpr(1) : new NumberExpr(0);
    if (op == "<") return left->getValue() < right->getValue() ? new NumberExpr(1) : new NumberExpr(0);
    if (op == "<=") return left->getValue() <= right->getValue() ? new NumberExpr(1) : new NumberExpr(0);
    if (op == "==") return left->getValue() == right->getValue() ? new NumberExpr(1) : new NumberExpr(0);
    if (op == "!=") return left->getValue() != right->getValue() ? new NumberExpr(1) : new NumberExpr(0);

    throw std::runtime_error("Unknown operator: " + op);
}

Expr* eval(Expr* expr, Env& env) {
    if (auto* number = dynamic_cast<NumberExpr*>(expr)) return expr;
    if (auto* symbol = dynamic_cast<SymbolExpr*>(expr)) return nullptr;
    if (auto* list = dynamic_cast<ListExpr*>(expr)) {
        if (list->size() == 0) throw std::runtime_error("Cannot evaluate empty list");

        auto head = list->get(0);
        auto* op = dynamic_cast<SymbolExpr*>(head);
        if (!op) throw std::runtime_error("First element must be an operator symbol");

        
        std::string opSymbol = op->getSymbol();
        
        if (opSymbol == "+" || opSymbol == "-" || opSymbol == "*" || opSymbol == "/") {
            if (list->size() != 3) throw std::runtime_error("Math operations require exactly 3 elements");
            
            auto* left = dynamic_cast<NumberExpr*>(eval(list->get(1), env));
            auto* right = dynamic_cast<NumberExpr*>(eval(list->get(2), env));
            if (!left || !right) throw std::runtime_error("Invalid operands for math operation");

            return evalMathOp(opSymbol, left, right);
        }

        if (opSymbol == ">" || opSymbol == ">=" || opSymbol == "<" || opSymbol == "<=" || opSymbol == "==" || opSymbol == "!=") {
            if (list->size() != 3) throw std::runtime_error("Comparison operations require exactly 3 elements");

            auto* left = dynamic_cast<NumberExpr*>(eval(list->get(1), env));
            auto* right = dynamic_cast<NumberExpr*>(eval(list->get(2), env));
            if (!left || !right) throw std::runtime_error("Invalid operands for comparison operation");

            return evalBoolOp(opSymbol, left, right);
        }

        if (op->getSymbol() == "if") {
            if (list->size() != 4) throw std::runtime_error("If statement requires exactly 3 arguments");

            auto* condition = eval(list->get(1), env);
            auto* thenBranch = list->get(2);
            auto* elseBranch = list->get(3);

            auto* conditionValue = dynamic_cast<NumberExpr*>(condition);
            if (!conditionValue) throw std::runtime_error("Condition must evaluate to a number");
            if (conditionValue->getValue() != 0) return eval(thenBranch, env);
            else return eval(elseBranch, env);
        }
    }

    throw std::runtime_error("Unknown expression type");
    return nullptr;
}
