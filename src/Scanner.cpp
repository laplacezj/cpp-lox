#include"Scanner.hpp"
#include "Lox.hpp"


Scanner::Scanner(const std::string& source)
{
    this->source = std::move(source);

    keywords = {
        {"and", TokenType::AND},     {"class", TokenType::CLASS},   {"else", TokenType::ELSE},
        {"false", TokenType::FALSE}, {"for", TokenType::FOR},       {"fun", TokenType::FUN},
        {"if", TokenType::IF},       {"nil", TokenType::NIL},       {"or", TokenType::OR},
        {"print", TokenType::PRINT}, {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
        {"this", TokenType::THIS},   {"true", TokenType::TRUE},     {"var", TokenType::VAR},
        {"while", TokenType::WHILE},
    };
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

bool Scanner::match(char expect)
{
    if(isAtEnd()) return false;
    if(source.at(current) != expect)return false;
    current++;

    return true;

}

char Scanner::peek()
{
    if (isAtEnd()) return '\0';
    return source.at(current);    
}

char Scanner::peekNext()
{
    if (current + 1 >= static_cast<int>(source.size())) return '\0';

    return source.at(current + 1);
}


void Scanner::string()
{
    while (peek()!='"' && isAtEnd())
    {
        if (peek()=='\n') line++;
        advance();
    }
    
    if (isAtEnd()) {
      lox::error(line, "Unterminated string.");
      return;
    }

    advance();

    std::string value = source.substr(start + 1, current - 1);
    addToken(STRING, value);
}

bool Scanner::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool Scanner::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}


void  Scanner::number()
{
    while (isDigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
      // Consume the "."
      advance();

      while (isDigit(peek())) advance();
    }

    addToken(TokenType::NUMBER, std::stod(source.substr(start, current - start)));

}

void Scanner::identifier()
{
    while (isAlphaNumeric(peek())) {
        advance();
    }

    const auto text = source.substr(start, current - start);
    if (const auto it = keywords.find(text); it != keywords.end()) {
        addToken(it->second);
    } else {
        addToken(TokenType::IDENTIFIER);
    }
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
    case '!':
        addToken(match('=') ? BANG_EQUAL : BANG);
        break;
      case '=':
        addToken(match('=') ? EQUAL_EQUAL : EQUAL);
        break;
      case '<':
        addToken(match('=') ? LESS_EQUAL : LESS);
        break;
      case '>':
        addToken(match('=') ? GREATER_EQUAL : GREATER);
        break;

    case '/':
        if (match('/'))
        {
            while (peek()!= '\n') advance();
        }
        else
        {
            addToken(SLASH);
        }
        break;

    case ' ':
    case '\r':
    case '\t':
        // Ignore whitespace.
        break;
    case '\n':
        line++;
        break;
    case '"':
        string();
        break;

    default:
        if (isDigit(c))
        {
            number();
        }
        else if (isAlpha(c))
        {
            identifier();
        }
        else
        {
            lox::error(line, "Unexpected character.");
        }
        
        
        
        break;
    }
}