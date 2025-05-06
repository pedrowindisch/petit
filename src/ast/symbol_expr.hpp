#include "ast/expr.hpp"
#include <string>

class SymbolExpr : public Expr {
    public:
        SymbolExpr(const std::string& s) : symbol(s) {}

        std::string getSymbol() const {
            return symbol;
        }
    private:
        std::string symbol;
};