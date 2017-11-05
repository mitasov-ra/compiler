
#include "compiler.h"
#include <iostream>

#include "Parser.h"


using namespace compiler;
using errs = SyntaxException::Errors;

void Parser::parse(const std::string &fileName) {
    math_expression();
}

void Parser::math_mult() {
    math_operand();
//    std::cout << "mult: Looking to the next id\n";
    Token tok = lexer.lookForToken();
//    std::cout << "mult: It is " << tok.type << " " << tok.id << std::endl;
    while (
            !(tok.type == ONE_LIT_DELIM &&
              (
                  tok.id == RPAREN ||
                  tok.id == LINE_DELIM ||
                  tok.id == PLUS ||
                  tok.id == MINUS
              )
            )
        ) {
//        std::cout << "mult: Getting the next id\n";
        tok = lexer.nextToken();
//        std::cout << "mult: It is " << tok.type << " " << tok.id << std::endl;
        if (tok.type == ONE_LIT_DELIM && tok.id == MULT) {
            math_operand();


        } else if (tok.type == ONE_LIT_DELIM && tok.id == MULT) {
            math_operand();


        } else {
            throw SyntaxException(errs::OPERAND_MISSING).setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
        }
        tok = lexer.lookForToken();
    }
}

void Parser::math_expression() {
    math_mult();
//    std::cout << "expr: Looking to the next token\n";
    Token tok = lexer.lookForToken();
//    std::cout << "expr: It is " << tok.type << " " << tok.id << std::endl;
    while (
            !(tok.type == ONE_LIT_DELIM &&
            (tok.id == RPAREN || tok.id == LINE_DELIM))
        ) {
//        std::cout << "expr: Getting the next token\n";
        tok = lexer.nextToken();
//        std::cout << "expr: It is " << tok.type << " " << tok.id << std::endl;
        if (tok.type == ONE_LIT_DELIM && tok.id == PLUS) {
            math_operand();


        } else if (tok.type == ONE_LIT_DELIM && tok.id == MINUS) {
            math_operand();


        } else {
            throw SyntaxException(errs::OPERAND_MISSING).setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
        }
        tok = lexer.lookForToken();
    }
}

void Parser::math_operand() {
//    std::cout << "op: Getting the next token\n";
    Token tok = lexer.nextToken();
//    std::cout << "op: It is " << tok.type << " " << tok.id << std::endl;
    if (tok.type == INTEGER) {
        //std::cout << "integer :" << tables->integers[tok.id] << std::endl;

        return;
    }

    if (tok.type == IDENTIFIER) {
//        std::cout << "identifier :" << tables->identifiers[tok.id] << std::endl;

        return;
    }

    if (tok.type == ONE_LIT_DELIM && tok.id == OneLitDelim::LPAREN) {
        math_expression();
        tok = lexer.nextToken();
        if (tok.type == ONE_LIT_DELIM && tok.id == OneLitDelim::RPAREN) {

            return;
        }
    }

    throw SyntaxException(errs::OPERAND_MISSING).setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
}
