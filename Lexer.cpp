#include "Lexer.h"

#include <utility>
#include <vector>

Keyword::Keyword(std::string *term, Lexeme lexeme) {
    this->term = term;
    this->lexeme = lexeme;
}

Keyword::~Keyword() {
    delete this->term;
}

Token::Token(Lexeme lexeme, const std::string *value) {
    this->lexeme = lexeme;
    this->value = value;
}

Token::~Token() {
    delete this->value;
}

Lexer::Lexer(const std::vector<Keyword *> *keywords, const std::string *valid_number_characters) {
    this->keywords = keywords;
    this->valid_number_characters = valid_number_characters;
}

Lexer::~Lexer() {
    delete this->keywords;
    delete this->valid_number_characters;
}

Token *Lexer::get_next_token(std::string &expression) const {
    Lexer::skip_spaces(expression);

    auto matching_keywords = new std::vector<Keyword *>(*this->keywords);
    bool is_number = true;

    std::string term;
    for (auto next_character : expression) {
        std::string next_term = term + next_character;

        auto next_matching_keywords = new std::vector<Keyword *>();
        for (auto keyword : *matching_keywords) {
            if (keyword->term->find(next_term) == 0) {
                next_matching_keywords->push_back(keyword);
            }
        }

        bool next_is_number = is_number && this->valid_number_characters->find(next_character) != std::string::npos;

        if (next_matching_keywords->empty() and not next_is_number) {
            break;
        }

        matching_keywords = next_matching_keywords;
        is_number = next_is_number;
        term = std::string(next_term);
    }

    expression.erase(0, term.length());
    if (is_number and not term.empty()) {
        return new Token(Lexeme::NUMBER, new std::string(term));
    }

    if (matching_keywords->size() != 1) {
        throw std::runtime_error("Ошибка анализа выражения: невозможно распознать \"" + term + "\"");
    }
    return new Token(matching_keywords->at(0)->lexeme, new std::string(term));
}


std::vector<Token *> *Lexer::tokenize(const std::string &original_expression) {
    auto expression = original_expression;
    auto tokens = new std::vector<Token *>();

    while (not expression.empty()) {
        tokens->push_back(this->get_next_token(expression));
    }

    return tokens;
}

void Lexer::skip_spaces(std::string &expression) {
    int spaces_count = 0;

    for (auto next_character : expression) {
        if (isspace(next_character)) {
            spaces_count++;
        } else {
            break;
        }
    }

    expression.erase(0, spaces_count);
}
