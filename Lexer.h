#ifndef CUSTOM_NOTATION_CALCULATOR_LEXER_H
#define CUSTOM_NOTATION_CALCULATOR_LEXER_H

#include <string>

enum Lexeme {
    NUMBER,
    PLUS, MINUS, DIVIDE, MULTIPLY,
};


class Keyword {
public:
    std::string *term;
    Lexeme lexeme;

    Keyword(std::string *term, Lexeme lexeme);

    ~Keyword();
};


class Token {
public:
    Lexeme lexeme;
    const std::string *value;

    Token(Lexeme lexeme, const std::string *value);

    ~Token();
};

class Lexer {
public:
    const std::vector<Keyword *> *keywords;
    const std::string *valid_number_characters;

    Lexer(const std::vector<Keyword *> *keywords, const std::string *valid_number_characters);

    ~Lexer();

    std::vector<Token *> *tokenize(const std::string &expression);

protected:
    [[nodiscard]] Token *get_next_token(std::string &expression) const;

    static void skip_spaces(std::string &expression);
};


#endif //CUSTOM_NOTATION_CALCULATOR_LEXER_H
