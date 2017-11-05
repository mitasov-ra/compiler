//
// Created by ROMAN on 03.11.2017.
//

#ifndef COMPILER_H
#define COMPILER_H

namespace compiler
{
    /*
     * Перечисление всех возможных типов токенов
     */
    enum TokenType {
        KEY_WORD,
        ONE_LIT_DELIM,
        TWO_LIT_DELIM,
        INTEGER,
        IDENTIFIER,
        LITERAL,
    };

    enum OneLitDelim {
        INPUT_END,
        LINE_DELIM,
        PLUS,
        MINUS,
        MULT,
        DIV,
        POW,
        ASSIGN,
        LPAREN,
        RPAREN,
        LBRACKET,
        RBRACKET,
        LBRACE,
        RBRACE,
        MORE,
        LESS,
        NOT,
        COMMA,
    };

    enum TwoLitDelim {
        PLUS_ASSIGN,
        MINUS_ASSIGN,
        DIV_ASSIGN,
        MULT_ASSIGN,
        INCR,
        DECR,
        EQUALS,
        MORE_OR_EQ,
        LESS_OR_EQ,
        NOT_EQ,
    };

    enum KeyWord {
        KEY_PROGRAM,
        KEY_VAR,
        KEY_IF,
        KEY_PRINT,
    };
}
#include "TokenTables.h"
#include "Lexer.h"
#include "Parser.h"
#include "SyntaxException.h"
#include "Token.h"

//#include "Lexer.cpp"
//#include "Parser.cpp"
//#include "SyntaxException.cpp"
//#include "Token.cpp"
//#include "TokenTables.cpp"


#endif //COMPILER_H
