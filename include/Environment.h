#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>

#include "Token.h"
#include "RuntimeError.h"

class Environment
{
private:
    std::map<std::string, std::any> values;
    std::shared_ptr<Environment> enclosing;

public:
    Environment()
        : enclosing{nullptr}
    {
    }

    Environment(std::shared_ptr<Environment> enclosing)
        : enclosing{std::move(enclosing)}
    {
    }
    
    ~Environment();

    void define(const std::string &name, std::any value);
    std::any get(Token name);
    void assign(const Token &name, std::any value);
};
