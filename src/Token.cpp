#include"Token.hpp"


Token::Token(TokenType type, std::string lexeme, std::any literal, int line):type(type), lexeme(std::move(lexeme)), literal(literal), line(line)
{

}

Token::~Token()
{

}



std::string Token::toString() const {
    return std::to_string(static_cast<int>(type)) + ", lexeme: '" + lexeme +
           "' , literal: '" + literalToString() + "'";
}


std::string Token::literalToString() const {
    switch (type) {
        case TokenType::STRING:
            return std::any_cast<std::string>(literal);
        case TokenType::NUMBER:
            return std::to_string(std::any_cast<double>(literal));
        default:
            return "";
    }
}