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
    enum TokenType
    {
        KEY_WORD,
        ONE_LIT_DELIM,
        TWO_LIT_DELIM,
        INTEGER,
        IDENTIFIER,
        LITERAL,
    };

    enum OneLitDelim
    {
        INPUT_END,
        LINESEP,
        PLUS,
        MINUS,
        MULT,
        DIV,
        //POW,
            ASSIGN,
        LPAREN,
        RPAREN,
        LBRACKET,
        RBRACKET,
//        LBRACE,
//        RBRACE,
        MORE,
        LESS,
        NOT,
        COMMA,
        EXPR_SEP,
    };

    enum TwoLitDelim
    {
        PLUS_ASSIGN,
        MINUS_ASSIGN,
        DIV_ASSIGN,
        MULT_ASSIGN,
        INCR,
        DECR,
        OR,
        AND,
        EQUALS,
        MORE_OR_EQ,
        LESS_OR_EQ,
        NOT_EQ,
    };

    enum KeyWord
    {
        KEY_PROGRAM,
        KEY_DIM,
        KEY_IF,
        KEY_ELSE,
        KEY_THEN,
        KEY_WRITE,
        KEY_READ,
        KEY_TRUE,
        KEY_FALSE,
        KEY_ARRAY,
        KEY_INT,
        KEY_STRING,
        KEY_OF,
        KEY_WHILE,
        KEY_DO,
        KEY_START,
        KEY_END,
//         KEY_FLOAT,

//        KEY_AND,
//        KEY_BEGIN,
//        KEY_CASE,
//        KEY_CONST,
//        KEY_DIV,
//        KEY_DOWNTO,
//        KEY_END,
//        KEY_FILE,
//        KEY_FOR,
//        KEY_FUNCTION,
//        KEY_GOTO,
//        KEY_IN,
//        KEY_LABEL,
//        KEY_MOD,
//        KEY_NIL,
//        KEY_NOT,
//        KEY_OR,
//        KEY_WITH,
//        KEY_REPEAT,
//        KEY_UNTIL,
//        KEY_RECORD,
//        KEY_PROCEDURE,
//        KEY_PACKED,
//        KEY_SET,
//        KEY_TO,
//        KEY_TYPE,
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
//#include "TokenTables.cpp"


#endif //COMPILER_H
