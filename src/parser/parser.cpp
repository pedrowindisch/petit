#include "parser.hpp"
#include "ast/expr.hpp"
#include "ast/number_expr.hpp"
#include "ast/symbol_expr.hpp"
#include "ast/list_expr.hpp"

std::vector<Expr*> Parser::parse() {
    std::vector<Expr*> expressions;
    while (current < tokens.size())
        expressions.push_back(parseExpression());
    
    return expressions;
}

Expr* Parser::parseExpression() {
    if (tokens[current] == "(") return parseList(); 
    return parseAtom(); 
}

Expr* Parser::parseList() {
    current++;  // "(" skipped

    std::vector<Expr*> elements;
    while (tokens[current] != ")") elements.push_back(parseExpression());

    current++;  // Skip the ")"
    return new ListExpr(elements);
}

Expr* Parser::parseAtom() {
    std::string token = tokens[current++];

    try {
        size_t pos = 0;
        double value = std::stod(token, &pos);
        
        // If pos is equal to the token length, the entire string was converted
        if (pos == token.length()) {
            return new NumberExpr(value);  // Valid number (integer or double)
        } else {
            throw std::invalid_argument("Invalid number format");
        }
    } catch (const std::invalid_argument& e) {
        // If parsing fails, it must be a symbol
        return new SymbolExpr(token);
    }
}