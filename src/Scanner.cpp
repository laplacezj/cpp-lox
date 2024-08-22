#include "Scanner.h"
#include "Error.h"

Scanner::Scanner(std::string_view source)
    : source(source)
{
}

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }

    tokens.emplace_back(END_OF_FILE, "", nullptr, line);
    return tokens;
}

void Scanner::addToken(TokenType type, std::any literal)
{
    std::string text{source.substr(start, current - start)};
    tokens.emplace_back(type, std::move(text), std::move(literal),
                        line);
}

bool Scanner::match(char expected)
{
    if (isAtEnd())
    {
        return false;
    }

    if (source[current] != expected)
        return false;

    ++current;
    return true;
}

void Scanner::string()
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n')
            line++;
        advance();
    }

    if (isAtEnd())
    {
        error(line, "Unterminated string.");
    }

    advance();
    std::string value{source.substr(start + 1, current - 2 - start)};

    addToken(STRING, value);
}

void Scanner::number()
{
    while (isdigit(peek()))
    {
        advance();
    }

    if (peek() == '.' && isdigit(peekNext()))
    {
        advance();
        while (isDigit(peek()))
            advance();
    }

    addToken(NUMBER,
             std::stod(std::string{
                 source.substr(start, current - start)}));
}

void Scanner::identifier()
{
    while (isAlphaNumeric(peek()))
        advance();

    // addToken(IDENTIFIER);

    std::string text =
        std::string{source.substr(start, current - start)};

    TokenType type;
    auto match = keywords.find(text);
    if (match == keywords.end())
    {
        type = IDENTIFIER;
    }
    else
    {
        type = match->second;
    }

    addToken(type);
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
            while (peek() != '\n' && !isAtEnd())
            {
                advance();
            }
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
    case '"':
        string();
        break;
    case '\n':
        line++;
        break;

    case 'o':
        if (match('r'))
        {
            addToken(OR);
        }
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
            error(line, "Unexpected character.");
        }
        break;
    }
}



const std::map<std::string, TokenType> Scanner::keywords =
{
  {"and",    TokenType::AND},
  {"class",  TokenType::CLASS},
  {"else",   TokenType::ELSE},
  {"false",  TokenType::FALSE},
  {"for",    TokenType::FOR},
  {"fun",    TokenType::FUN},
  {"if",     TokenType::IF},
  {"nil",    TokenType::NIL},
  {"or",     TokenType::OR},
  {"print",  TokenType::PRINT},
  {"return", TokenType::RETURN},
  {"super",  TokenType::SUPER},
  {"this",   TokenType::THIS},
  {"true",   TokenType::TRUE},
  {"var",    TokenType::VAR},
  {"while",  TokenType::WHILE},
};