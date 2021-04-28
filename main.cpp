#include <iostream>
#include <vector>
#include <unordered_map>
#include "Lexer.h"
#include "Parser.h"

int main() {
    std::cout << "Куркулятор выражений в обратной польской нотации" << std::endl;
    auto expression = new std::string();

    std::cout << "Введите выражение в обратной польмкой нотации: ";
    std::getline(std::cin, *expression);

    auto keywords = new std::vector<Keyword *>{
            new Keyword(new std::string("+"), Lexeme::PLUS),
            new Keyword(new std::string("-"), Lexeme::MINUS),
            new Keyword(new std::string("*"), Lexeme::MULTIPLY),
            new Keyword(new std::string("/"), Lexeme::DIVIDE),
    };
    auto lexer = new Lexer(keywords, new std::string("0123456789"));
    auto tokens = lexer->tokenize(*expression);

    auto operation_constructors = new std::unordered_map<Lexeme, BinaryOperation *(*)(Node *, Node *)>{
            {
                    Lexeme::PLUS,
                    [](Node *left, Node *right) {
                        return new BinaryOperation(
                                left,
                                right,
                                [](double a, double b) {
                                    return a + b;
                                },
                                new std::string("+"),
                                true
                        );
                    }
            },
            {
                    Lexeme::MINUS,
                    [](Node *left, Node *right) {
                        return new BinaryOperation(
                                left,
                                right,
                                [](double a, double b) {
                                    return a - b;
                                },
                                new std::string("-"),
                                true
                        );
                    }
            },
            {
                    Lexeme::MULTIPLY,
                    [](Node *left, Node *right) {
                        return new BinaryOperation(
                                left,
                                right,
                                [](double a, double b) {
                                    return a * b;
                                },
                                new std::string("*"),
                                false
                        );
                    }
            },
            {
                    Lexeme::DIVIDE,
                    [](Node *left, Node *right) {
                        return new BinaryOperation(
                                left,
                                right,
                                [](double a, double b) {
                                    return a / b;
                                },
                                new std::string("/"),
                                false
                        );
                    }
            },
    };
    auto parser = new Parser(operation_constructors);
    auto root_node = parser->parse(*tokens);

    std::cout << "Префиксная запись: " << *root_node->unparse_prefix() << std::endl;
    std::cout << "Инфиксная запись: " << *root_node->unparse_infix() << std::endl;
    std::cout << "Результат: " << root_node->get_value() << std::endl;

    for(auto token: *tokens) {
        delete token;
    }
    delete tokens;
    delete lexer;
    delete parser;
    delete root_node;

    return 0;
}
