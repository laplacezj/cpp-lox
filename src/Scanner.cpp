#include"Scanner.hpp"

Scanner::Scanner(const std::string& source)
{
    this->source = std::move(source);
}

Scanner::~Scanner()
{
}


std::vector<Token> Scanner::scanTokens()
{
    while (isAtEnd())
    {
        start = current;
        scanToken();
    }

    tokens.emplace_back(TokenType::TOKEN_EOF, "",std::any{},line);
    return tokens;
    
}

bool Scanner::isAtEnd() const
{
    return current >= static_cast<int>(tokens.size());
}


char Scanner::advance()
{
    ++current;
    return source.at(current - 1);
}

void Scanner::addToken(TokenType type, std::any literal)
{
    std::string text = source.substr(start,current-start);
    tokens.emplace_back(Token{type, text, literal, line});
}
void Scanner::addToken(TokenType type)
{
    addToken(type, std::any{});
}



void Scanner::scanToken()
{
    char c = advance();

    switch (c)
    {
    case '(':
        addToken(LEFT_PAREN);
        break;

    case ')': addToken(RIGHT_PAREN); break;
    case '{': addToken(LEFT_BRACE); break;
    case '}': addToken(RIGHT_BRACE); break;
    case ',': addToken(COMMA); break;
    case '.': addToken(DOT); break;
    case '-': addToken(MINUS); break;
    case '+': addToken(PLUS); break;
    case ';': addToken(SEMICOLON); break;
    case '*': addToken(STAR); break; 

    default:
        break;
    }
}