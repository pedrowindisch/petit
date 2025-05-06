#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "ast/expr.hpp"

class Env {
    public:
        Env() = default;

        void set(const std::string& name, Expr* value) {
            env[name] = value;
        }

        Expr* get(const std::string& name) {
            return env[name];
        }

    private:
        std::unordered_map<std::string, Expr*> env;
};
