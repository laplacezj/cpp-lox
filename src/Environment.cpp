#include "Environment.h"

Environment::~Environment()
{
}

void Environment::define(const std::string &name, std::any value)
{
    values[name] = std::move(value);
}

std::any Environment::get(Token name)
{
    auto elem = values.find(name.lexeme);
    if (elem != values.end())
    {
        return elem->second;
    }

    if (enclosing != nullptr) return enclosing->get(name);

    throw RuntimeError(name,
                       "Undefined variable '" + name.lexeme + "'.");
}

void Environment::assign(const Token &name, std::any value)
{
    auto elem = values.find(name.lexeme);

    if (elem != values.end())
    {
        elem->second = std::move(value);
        return;
    }

    if (enclosing != nullptr) {
      enclosing->assign(name, std::move(value));
      return;
    }

    throw RuntimeError(name,
                       "Undefined variable '" + name.lexeme + "'.");

}

