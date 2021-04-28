cmake_minimum_required(VERSION 3.17)
project(custom_notation_calculator)

set(CMAKE_CXX_STANDARD 17)

add_executable(custom_notation_calculator main.cpp Lexer.cpp Lexer.h Parser.cpp Parser.h)