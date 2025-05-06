#include "lexer.hpp"
#include <cctype>

std::vector<std::string> tokenize(const std::string& source) {
    std::vector<std::string> tokens;
    std::string token;
    for (char c : source) {
        if (std::isspace(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else if (c == '(' || c == ')') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(std::string(1, c));
        } else {
            token += c;
        }
    }
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}
