#include <vector>
#include "ast/expr.hpp"
#include <string>

class ListExpr : public Expr {
    public:
        ListExpr(const std::vector<Expr*>& elems) : elements(elems) {}
    
        // Optional: method to add an element to the list
        void addElement(Expr* elem) {
            elements.push_back(elem);
        }

        Expr* get(size_t index) const {
            return elements[index];
        }

        size_t size() const {
            return elements.size();
        }
    
        std::vector<Expr*> elements;
};