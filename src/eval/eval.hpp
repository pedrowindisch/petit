#pragma once

#include "env/env.hpp"
#include "ast/expr.hpp"

Expr* eval(Expr* expr, Env& env);
