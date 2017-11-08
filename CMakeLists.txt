cmake_minimum_required(VERSION 3.8)
project(Compiler)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        src/main.cpp src/Lexer.cpp src/Lexer.h src/SyntaxException.cpp src/SyntaxException.h src/Token.h src/Parser.cpp src/Parser.h src/TokenTables.cpp src/TokenTables.h src/compiler.h)

add_executable(Compiler ${SOURCE_FILES})

