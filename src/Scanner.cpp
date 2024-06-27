#include "lox/Scanner.hpp"
#include "lox/Lox.hpp"
#include <cctype> // isdigit, isalpha

namespace Lox
{
    Scanner::Scanner(const std::string &source)
    {
        this->source = std::move(source);

        keywords = {
            {"and", TokenType::AND},
            {"class", TokenType::CLASS},
            {"else", TokenType::ELSE},
            {"false", TokenType::FALSE},
            {"for", TokenType::FOR},
            {"fun", TokenType::FUN},
            {"if", TokenType::IF},
            {"nil", TokenType::NIL},
            {"or", TokenType::OR},
            {"print", TokenType::PRINT},
            {"return", TokenType::RETURN},
            {"super", TokenType::SUPER},
            {"this", TokenType::THIS},
            {"true", TokenType::TRUE},
            {"var", TokenType::VAR},
            {"while", TokenType::WHILE},
        };
    }

    Scanner::~Scanner()
    {
    }

    std::vector<Token> Scanner::scanTokens()
    {
        //std::cout<<"start: "<<start<< "  end:"<<current<<" line:"<<line<<std::endl;
        while (!isAtEnd())
        {
            start = current;
            scanToken();
        }

        tokens.emplace_back(TokenType::TOKEN_EOF, "", line);
        return tokens;
    }

    bool Scanner::isAtEnd() const
    {
        return current >= static_cast<int>(source.size());
    }

    char Scanner::advance()
    {
        ++current;
        return source.at(current - 1);
    }

    void Scanner::addToken(TokenType type, std::any literal)
    {
        std::string text = source.substr(start, current - start);
        tokens.emplace_back(Token{type, text, literal, line});
    }

    void Scanner::addToken(TokenType type)
    {
        tokens.emplace_back(type, source.substr(start, current - start), line);
    }

    void Scanner::comment()
    {
        // A comment goes until the end of the line.
        while (peek() != '\n' && !isAtEnd())
        {
            advance();
        }
    }

    bool Scanner::match(char expect)
    {
        if (isAtEnd())
            return false;
        if (source.at(current) != expect)
            return false;
        current++;

        return true;
    }

    char Scanner::peek() const
    {
        if (isAtEnd())
            return '\0';
        return source.at(current);
    }

    char Scanner::peekNext() const
    {
        if (current + 1 >= static_cast<int>(source.size()))
            return '\0';

        return source.at(current + 1);
    }

    void Scanner::string()
    {
        while (peek() != '"' && !isAtEnd())
        {
            if (peek() == '\n')line++;
            advance();
        }

        if (isAtEnd())
        {
            lox::error(line, "Unterminated string.");
            return;
        }

        advance();

        std::string value = source.substr(start + 1, current - start - 2);
        addToken(TokenType::STRING, value);
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

    void Scanner::number()
    {
        while (std::isdigit(peek()))
            advance();

        // Look for a fractional part.
        if (peek() == '.' && std::isdigit(peekNext()))
        {
            // Consume the "."
            advance();

            while (std::isdigit(peek()))
                advance();
        }

        addToken(TokenType::NUMBER, std::stod(source.substr(start, current - start)));
    }

    void Scanner::identifier()
    {
        while (std::isdigit(peek()) || std::isalpha(peek()) || peek() == '_')
        {
            advance();
        }

        const auto text = source.substr(start, current - start);
        if (const auto it = keywords.find(text); it != keywords.end())
        {
            addToken(it->second);
        }
        else
        {
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

        case ')':
            addToken(RIGHT_PAREN);
            break;
        case '{':
            addToken(LEFT_BRACE);
            break;
        case '}':
            addToken(RIGHT_BRACE);
            break;
        case ',':
            addToken(COMMA);
            break;
        case '.':
            addToken(DOT);
            break;
        case '-':
            addToken(MINUS);
            break;
        case '+':
            addToken(PLUS);
            break;
        case ';':
            addToken(SEMICOLON);
            break;
        case '*':
            addToken(STAR);
            break;
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
                comment();
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
            if (std::isdigit(c))
            {
                number();
            }
            else if (std::isalpha(c) || c=='_')
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
}
