
#include "compiler.h"
#include <iostream>

#include "Parser.h"


using namespace compiler;
using errs = SyntaxException::Errors;

void Parser::parse(const std::string &fileName) {
    program();
    if (lexer.lookForToken() == Token{KEY_WORD, KEY_VAR}) {
        var();
    }
    block();
    if (lexer.nextToken() != Token{ONE_LIT_DELIM, INPUT_END})
        throw SyntaxException(errs::CODE_AFTER_END)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
}

void Parser::program() {
    if (lexer.nextToken() != Token{KEY_WORD, KEY_PROGRAM})
        throw SyntaxException(errs::KEYWORD_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
                .setMessage("program: не найдено начало программы");

    if (lexer.nextToken().type == IDENTIFIER) {

    } else {
        throw SyntaxException(errs::IDENTIFIER_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
                .setMessage("Не указано имя программы");
    }
    line_sep();
}

void Parser::var() {
    if (lexer.nextToken() != Token{KEY_WORD, KEY_VAR}) {
        throw SyntaxException(errs::KEYWORD_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
                .setMessage("Не найден var");
    }
    auto tok = lexer.lookForToken();
    do {

        if (tok == Token{KEY_WORD, KEY_ARRAY}) {
            lexer.nextToken();
            assignment();
            line_sep();
        } else if (tok == Token{KEY_WORD, KEY_INT}) {
            lexer.nextToken();
            assignment();
            line_sep();
        } else if (tok == Token{KEY_WORD, KEY_STRING}) {
            lexer.nextToken();
            assignment();
            line_sep();
        } else {
            throw SyntaxException(errs::KEYWORD_MISSING)
                    .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
                    .setMessage("Пропущен тип переменной");
        }
        tok = lexer.lookForToken();

    } while (tok != Token{ONE_LIT_DELIM, LBRACE});
}

void Parser::block() {
    if (lexer.nextToken() != Token{ONE_LIT_DELIM, LBRACE}) {
        throw SyntaxException(errs::LBRACE_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
    }

    auto tok = lexer.lookForToken();
    while (tok != Token{ONE_LIT_DELIM, RBRACE}) {
        if (tok.type == IDENTIFIER || tok == Token{ONE_LIT_DELIM, LPAREN}) {
            assignment();
            tok = lexer.lookForToken();
            line_sep();
        } else if (tok == Token{KEY_WORD, KEY_IF}) {
            lexer.nextToken();
            if_statement();

        } else if (tok == Token{KEY_WORD, KEY_PRINT}) {
            lexer.nextToken();
            print_statement();

        } else {
            throw SyntaxException(errs::UNEXPECTED_TOKEN)
                    .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
        }
        tok = lexer.lookForToken();
    }

    if (lexer.nextToken() != Token{ONE_LIT_DELIM, RBRACE}) {
        throw SyntaxException(errs::RBRACE_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
    }
}

void Parser::line_sep() {
    if (lexer.nextToken() != Token{ONE_LIT_DELIM, LINESEP})
        throw SyntaxException(errs::LINESEP_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
}

void Parser::if_statement() {
    assignment();
    auto tok = lexer.nextToken();
    if (tok != Token{KEY_WORD, KEY_THEN}) {
        throw SyntaxException(errs::KEYWORD_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
                .setMessage("Пропущен then");
    }
    block();
    if (lexer.lookForToken() == Token{KEY_WORD, KEY_ELSE}) {
        lexer.nextToken();
        else_statement();
    }
}

void Parser::else_statement() {
    if (lexer.lookForToken() == Token{KEY_WORD, KEY_IF}) {
        lexer.nextToken();
        if_statement();
    } else {
        block();
    }
}

void Parser::print_statement() {
    assignment();
    line_sep();
}


void Parser::assignment() {
    or_expr();

    auto tok = lexer.lookForToken();

    while (!followsAssigment(tok)) {
        tok = lexer.nextToken();

        if (tok == Token{ONE_LIT_DELIM, ASSIGN}) {
            or_expr();

        } else {
            throw SyntaxException(errs::OPERATOR_MISSING)
                    .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
                    .setMessage("assignment");
        }
        tok = lexer.lookForToken();
    }
}


void Parser::or_expr() {
    and_expr();
    Token tok = lexer.lookForToken();
    while (!followsOr(tok)) {
        tok = lexer.nextToken();
        if (tok == Token{ONE_LIT_DELIM, OR}) {
            and_expr();

        } else {
            throw SyntaxException(errs::OPERATOR_MISSING)
                    .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
                    .setMessage("or");
        }
        tok = lexer.lookForToken();
    }
}

void Parser::and_expr() {
    equation();
    Token tok = lexer.lookForToken();
    while (!followsAnd(tok)) {
        tok = lexer.nextToken();
        if (tok == Token{ONE_LIT_DELIM, AND}) {
            equation();

        } else {
            throw SyntaxException(errs::OPERATOR_MISSING)
                    .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
                    .setMessage("and");
        }
        tok = lexer.lookForToken();
    }
}

void Parser::equation() {
    relation();

    auto tok = lexer.lookForToken();
    while (!followsEquation(tok)) {
        tok = lexer.nextToken();
        if (tok == Token{TWO_LIT_DELIM, EQUALS}) {
            relation();


        } else if (tok == Token{TWO_LIT_DELIM, NOT_EQ}) {
            relation();


        } else {
            throw SyntaxException(errs::OPERATOR_MISSING)
                    .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
                    .setMessage("eq");
        }
        tok = lexer.lookForToken();
    }
}

void Parser::relation() {
    add_expr();

    auto tok = lexer.lookForToken();
    while (!followsRelation(tok)) {
        tok = lexer.nextToken();
        if (tok == Token{ONE_LIT_DELIM, MORE}) {
            add_expr();

        } else if (tok == Token{ONE_LIT_DELIM, LESS}) {
            add_expr();

        } else if (tok == Token{TWO_LIT_DELIM, LESS_OR_EQ}) {
            add_expr();

        } else if (tok == Token{TWO_LIT_DELIM, MORE_OR_EQ}) {
            add_expr();

        } else {
            throw SyntaxException(errs::OPERATOR_MISSING)
                    .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
                    .setMessage("rel");
        }
        tok = lexer.lookForToken();
    }
}

void Parser::add_expr() {
    mul_expr();
    Token tok = lexer.lookForToken();
    while (!followsAdd(tok)) {
        tok = lexer.nextToken();
        if (tok == Token{ONE_LIT_DELIM, PLUS}) {
            mul_expr();

        } else if (tok == Token{ONE_LIT_DELIM, MINUS}) {
            mul_expr();

        } else {
            throw SyntaxException(errs::OPERATOR_MISSING)
                    .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
                    .setMessage("add");
        }
        tok = lexer.lookForToken();
    }
}

void Parser::mul_expr() {
    unary_expr();
    auto tok = lexer.lookForToken();
    while (!followsMul(tok)) {
        tok = lexer.nextToken();
        if (tok == Token{ONE_LIT_DELIM, DIV}) {
            primary_expr();

        } else if (tok == Token{ONE_LIT_DELIM, MULT}) {
            primary_expr();

        } else {
            throw SyntaxException(errs::OPERATOR_MISSING)
                    .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
                    .setMessage("mul");
        }
        tok = lexer.lookForToken();
    }
}

void Parser::unary_expr() {
    auto tok = lexer.lookForToken();
    if (tok == Token{ONE_LIT_DELIM, MINUS}) {
        lexer.nextToken();
    } else if (tok == Token{ONE_LIT_DELIM, NOT}) {

    } else if (tok == Token{TWO_LIT_DELIM, INCR}) {

    } else if (tok == Token{TWO_LIT_DELIM, DECR}) {

    }

    postfix_expr();
}

void Parser::postfix_expr() {
    primary_expr();
    auto tok = lexer.lookForToken();
    while (!followsPostfixExpr(tok)) {
        if (tok == Token{ONE_LIT_DELIM, LBRACKET}) {
            lexer.nextToken();
            assignment();
            if (lexer.nextToken() != Token{ONE_LIT_DELIM, RBRACKET}) {
                throw SyntaxException(errs::RBRACKET_MISSING)
                        .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
            }
        }


        else {
            throw SyntaxException(errs::OPERATOR_MISSING)
                    .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
                    .setMessage("post_e");
        }
        tok = lexer.lookForToken();
    }
}



void Parser::primary_expr() {
    auto tok = lexer.nextToken();

    if (tok == Token{KEY_WORD, KEY_TRUE}) {

        return;
    }

    if (tok == Token{KEY_WORD, KEY_FALSE}) {

        return;
    }

    if (tok.type == INTEGER) {

        return;
    }

    if (tok.type == IDENTIFIER) {

        return;
    }

    if (tok.type == LITERAL) {

        return;
    }

    if (tok == Token{ONE_LIT_DELIM, OneLitDelim::LPAREN}) {
        assignment();
        if (lexer.nextToken() == Token{ONE_LIT_DELIM, OneLitDelim::RPAREN}) {

            return;
        }
    }
    throw SyntaxException(errs::OPERAND_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
            .setMessage("Ожидалось число, переменная или '('");

}

bool Parser::followsAssigment(const Token &tok) const noexcept {
    return (tok.type == ONE_LIT_DELIM &&
            tok.id == RPAREN ||
            tok.id == RBRACKET ||
            tok.id == LINESEP) ||
           tok == Token{KEY_WORD, KEY_THEN};
}

bool Parser::followsOr(const Token &tok) const noexcept {
    return followsAssigment(tok) || tok.type == ONE_LIT_DELIM && tok.id == ASSIGN;
}

bool Parser::followsAnd(const Token &tok) const noexcept {
    return followsOr(tok) || tok.type == ONE_LIT_DELIM && tok.id == OR;
}

bool Parser::followsEquation(const Token &tok) const noexcept {
    return followsAnd(tok) || tok.type == ONE_LIT_DELIM && tok.id == AND;
}

bool Parser::followsRelation(const Token &tok) const noexcept {
    return followsEquation(tok) || tok.type == TWO_LIT_DELIM && tok.id == EQUALS || tok.id == NOT_EQ;
}

bool Parser::followsAdd(const Token &tok) const noexcept {
    return followsRelation(tok) || (tok.type == ONE_LIT_DELIM && tok.id == MORE || tok.id == LESS) ||
           (tok.type == TWO_LIT_DELIM && tok.id == MORE_OR_EQ || tok.id == LESS_OR_EQ);
}

bool Parser::followsMul(const Token &tok) const noexcept {
    return followsAdd(tok) || tok.type == ONE_LIT_DELIM && tok.id == PLUS || tok.id == MINUS;
}

bool Parser::followsPostfixExpr(const Token &tok) const noexcept {
    return followsMul(tok);
}


