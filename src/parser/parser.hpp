#pragma once

#include <string>
#include <vector>
#include "ast/expr.hpp"

class Parser {
    public:
        Parser(const std::vector<std::string>& tokens) : tokens(tokens), current(0) {}

        std::vector<Expr*> parse();

    private:
        std::vector<std::string> tokens;
        size_t current;

        Expr* parseExpression();
        Expr* parseAtom();
        Expr* parseList();
};