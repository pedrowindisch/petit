#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "lexer/lexer.hpp"
#include "parser/parser.hpp" 
#include "eval/eval.hpp"
#include "ast/number_expr.hpp"
#include "ast/symbol_expr.hpp"
#include "ast/list_expr.hpp"
#include "env/env.hpp"
#include "ast/expr.hpp"

void repl() {
    std::string line;
    std::cout << "petit> ";
    while (std::getline(std::cin, line)) {
        try {
            auto tokens = tokenize(line);

            Parser parser(tokens);  
            auto exprs = parser.parse();

            Env env;

            // auto result = eval(expr, env);
            if (exprs.empty()) {
                std::cerr << "Error: No expressions found." << std::endl;
                continue;
            }
            
            for (auto& expr : exprs) {
                auto result = eval(expr, env);
                if (result == nullptr) {
                    std::cerr << "Error: Evaluation returned null." << std::endl;
                    continue;
                }

                if (auto* number = dynamic_cast<NumberExpr*>(result)) std::cout << number->getValue() << std::endl;
                else if (auto* symbol = dynamic_cast<SymbolExpr*>(result)) std::cout << symbol->getSymbol() << std::endl;
                else std::cout << "Unknown result type" << std::endl;
            }
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
        std::cout << "petit> ";
    }
}

void script(const std::string& filename) {
    std::ifstream file(filename);   
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    if (file.fail()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    auto tokens = tokenize(source);
    if (tokens.empty()) {
        std::cerr << "Error: No tokens found in file." << std::endl;
        return;
    }
    Parser parser(tokens);
    auto expr = parser.parse();

    Env env;
    if (expr.empty()) {
        std::cerr << "Error: No expressions found in file." << std::endl;
        return;
    }

    for (auto& e : expr) {
        auto result = eval(e, env);
        if (result == nullptr) {
            std::cerr << "Error: Evaluation returned null." << std::endl;
            continue;
        }
        
        if (auto* number = dynamic_cast<NumberExpr*>(result)) std::cout << number->getValue() << std::endl;
        else if (auto* symbol = dynamic_cast<SymbolExpr*>(result)) std::cout << symbol->getSymbol() << std::endl;
        else std::cout << "Unknown result type" << std::endl;
    }

    if (file.bad()) std::cerr << "Error reading file: " << filename << std::endl;
    file.close();
}

int main(int argc, char* argv[]) {
    if (argc == 1) repl();
    else if (argc == 2) {
        std::string filename = argv[1];
        script(filename);
    } else {
        std::cerr << "Usage: petit [filename]" << std::endl;
        return 1;
    }

    return 0;
}
