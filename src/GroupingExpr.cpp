#include "lox/GroupingExpr.hpp"

#include<cassert>

namespace Lox
{
    GroupingExpr::GroupingExpr(std::unique_ptr<Expr> expr):
    expr (std::move(expr))
    {
        assert(expr != nullptr);
    }

} // namespace Lox
