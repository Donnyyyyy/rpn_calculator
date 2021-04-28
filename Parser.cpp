#include "Parser.h"

Node::~Node() = default;

Number::Number(int value) {
    this->value = value;
}

Number::~Number() = default;

double Number::get_value() {
    return this->value;
}

std::string *Number::unparse_prefix() {
    return new std::string(std::to_string(this->value));
}

std::string *Number::unparse_infix() {
    return new std::string(std::to_string(this->value));
}

BinaryOperation::BinaryOperation(
        Node *left_expression,
        Node *right_expression,
        double (*foo)(double, double),
        std::string *operation_symbol,
        bool is_low_priority
) {
    this->left_expression = left_expression;
    this->right_expression = right_expression;
    this->foo = foo;
    this->operation_symbol = operation_symbol;
    this->is_low_priority = is_low_priority;
}

BinaryOperation::~BinaryOperation() {
    delete this->left_expression;
    delete this->right_expression;
    delete this->operation_symbol;
}

double BinaryOperation::get_value() {
    double a = this->left_expression->get_value();
    double b = this->right_expression->get_value();

    return this->foo(a, b);
}

std::string *BinaryOperation::unparse_prefix() {
    auto result = *this->operation_symbol
                  + " "
                  + *this->left_expression->unparse_prefix()
                  + " "
                  + *this->right_expression->unparse_prefix();

    return new std::string(result);
}

std::string *BinaryOperation::unparse_infix() {
    auto result = *this->left_expression->unparse_infix()
                  + " "
                  + *this->operation_symbol
                  + " "
                  + *this->right_expression->unparse_infix();

    if (this->is_low_priority) {
        result = "(" + result + ")";
    }

    return new std::string(result);
}


Parser::Parser(std::unordered_map<Lexeme, BinaryOperation *(*)(Node *, Node *)> *operation_constructors) {
    this->operation_constructors = operation_constructors;
}

Parser::~Parser() {
    delete this->operation_constructors;
}

Node *Parser::parse(std::vector<Token *> &tokens) {
    if (tokens.empty()) {
//        Пустое выражение
        throw std::runtime_error("Ошибка выражения: введено пустое выражение");
    }

    auto last_token = tokens.at(tokens.size() - 1);

    if (last_token->lexeme == Lexeme::NUMBER) {
        tokens.pop_back();
        return new Number(std::stoi(*last_token->value, nullptr, 10));
    } else {
        auto operation_constructor = (*this->operation_constructors)[last_token->lexeme];

        auto operand_expression = new std::vector(tokens.begin(), tokens.end() - 1);
        auto left_operand = this->parse(*operand_expression);
        auto right_operand = this->parse(*operand_expression);

        auto operation = (*operation_constructor)(left_operand, right_operand);
        return operation;
    }
}
