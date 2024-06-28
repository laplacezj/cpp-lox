#include"lox/LiteralExpr.hpp"

namespace Lox
{
    LiteralExpr::LiteralExpr(std::any literal):
    literal(std::move(literal))
    {

    }
} // namespace Lox
