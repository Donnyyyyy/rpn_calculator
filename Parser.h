#ifndef CUSTOM_NOTATION_CALCULATOR_PARSER_H
#define CUSTOM_NOTATION_CALCULATOR_PARSER_H


#include <vector>
#include <set>
#include <unordered_map>
#include "Lexer.h"


class Node {
public:
    virtual ~Node() = 0;

    virtual double get_value() = 0;

    virtual std::string *unparse_prefix() = 0;

    virtual std::string *unparse_infix() = 0;
};

class Number : public Node {
public:
    int value;

    explicit Number(int value);

    ~Number() override;

    double get_value() override;

    std::string *unparse_prefix() override;

    std::string *unparse_infix() override;
};

class BinaryOperation : public Node {
public:
    Node *left_expression;
    Node *right_expression;

    double (*foo)(double, double);

    std::string *operation_symbol;
    bool is_low_priority;

    BinaryOperation(
            Node *left_expression,
            Node *right_expression,
            double (*foo)(double, double),
            std::string *operation_symbol,
            bool is_low_priority
    );

    ~BinaryOperation() override;

    double get_value() override;

    std::string *unparse_prefix() override;

    std::string *unparse_infix() override;
};


class Parser {
public:
    explicit Parser(std::unordered_map<Lexeme, BinaryOperation *(*)(Node *, Node *)> *operation_constructors);

    ~Parser();

    Node *parse(std::vector<Token *> &tokens);

protected:
    std::unordered_map<Lexeme, BinaryOperation *(*)(Node *, Node *)> *operation_constructors;
};

#endif //CUSTOM_NOTATION_CALCULATOR_PARSER_H
