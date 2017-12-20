#include <iostream>
#include "Parser.h"
#include "compiler.h"

using namespace compiler;
typedef SyntaxException::Errors errs;

void Parser::parse(const std::string &fileName)
{
    program();
    if (lexer.lookForToken().compare(KEY_WORD, KEY_DIM)) {
        var();
    }
    block();
    if (!lexer.nextToken().compare(ONE_LIT_DELIM, INPUT_END)) {
        throw SyntaxException(errs::CODE_AFTER_END)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
    }
}

void Parser::program()
{
    if (!lexer.nextToken().compare(KEY_WORD, KEY_PROGRAM)) {
        throw SyntaxException(errs::KEYWORD_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
            .setMessage("program: не найдено начало программы");
    }

    if (lexer.nextToken().type == IDENTIFIER) {

    } else {
        throw SyntaxException(errs::IDENTIFIER_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
            .setMessage("Не указано имя программы");
    }
    line_sep();
}

void Parser::var()
{
    if (!lexer.nextToken().compare(KEY_WORD, KEY_DIM)) {
        throw SyntaxException(errs::KEYWORD_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
            .setMessage("Не найден var");
    }
    auto tok = lexer.lookForToken();
    do {
        if (tok.compare(KEY_WORD, KEY_ARRAY)) {
            array_type();
        } else {
            type();
        }
        if (lexer.nextToken().type == IDENTIFIER) {

        }
        line_sep();

        tok = lexer.lookForToken();

    } while (!tok.compare(KEY_WORD, KEY_START));
}

void Parser::type()
{
    auto tok = lexer.lookForToken();
    if (tok.type == KEY_WORD) {
        switch (tok.id) {
        case KEY_INT:
            lexer.nextToken();

            break;
        case KEY_STRING:
            lexer.nextToken();

            break;
        default:
            goto type_error;
        }
    } else {
        type_error:
        throw SyntaxException(errs::KEYWORD_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
            .setMessage("Пропущен тип переменной");
    }
}

void Parser::array_type()
{
    lexer.nextToken();

    if (!lexer.nextToken().compare(ONE_LIT_DELIM, LBRACKET)) {
        throw SyntaxException(errs::LBRACKET_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
    }

    if (lexer.nextToken().type != INTEGER) {
        throw SyntaxException(errs::UNEXPECTED_TOKEN)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
    }

    //логика

    if (!lexer.nextToken().compare(ONE_LIT_DELIM, RBRACKET)) {
        throw SyntaxException(errs::RBRACKET_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
    }

    if (!lexer.nextToken().compare(KEY_WORD, KEY_OF)) {
        throw SyntaxException(errs::KEYWORD_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
            .setMessage("Не найдено of");
    }

    type();
}

void Parser::block()
{
    if (!lexer.nextToken().compare(KEY_WORD, KEY_START)) {
        throw SyntaxException(errs::LBRACE_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
    }

    statement();

    if (!lexer.nextToken().compare(KEY_WORD, KEY_END)) {
        throw SyntaxException(errs::RBRACE_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
    }
}

void Parser::statement()
{
    auto tok = lexer.lookForToken();
    while (!tok.compare(KEY_WORD, KEY_END)) {
        if (tok.compare(KEY_WORD, KEY_IF)) {

            if_statement();

        } else if (tok.compare(KEY_WORD, KEY_WRITE)) {

            print_statement();

        } else if (tok.compare(KEY_WORD, KEY_WHILE)) {
            while_statement();
        } else if (tok.compare(KEY_WORD, KEY_READ)) {
            read_statement();
        } else {
            assignment();
            line_sep();
        }
        tok = lexer.lookForToken();
    }
}

void Parser::line_sep()
{
    if (!lexer.nextToken().compare(ONE_LIT_DELIM, LINESEP)) {
        throw SyntaxException(errs::LINESEP_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
    }
}

void Parser::if_statement()
{
    lexer.nextToken();
    if (!lexer.nextToken().compare(ONE_LIT_DELIM, LPAREN)) {
        throw SyntaxException(errs::LPAREN_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
    }
    or_expr();
    lexer.nextToken();
    auto tok = lexer.nextToken();
    if (!tok.compare(KEY_WORD, KEY_THEN)) {
        throw SyntaxException(errs::KEYWORD_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
            .setMessage("Пропущен then");
    }
    block();
    if (lexer.lookForToken().compare(KEY_WORD, KEY_ELSE)) {
        else_statement();
    }
}

void Parser::else_statement()
{
    lexer.nextToken();
    if (lexer.lookForToken().compare(KEY_WORD, KEY_IF)) {
        if_statement();
    } else {
        block();
    }
}

void Parser::while_statement()
{
    lexer.nextToken();
    if (!lexer.nextToken().compare(ONE_LIT_DELIM, LPAREN)) {
        throw SyntaxException(errs::LPAREN_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
    }
    or_expr();
    lexer.nextToken();
    if (!lexer.nextToken().compare(KEY_WORD, KEY_DO)) {
        throw SyntaxException(errs::KEYWORD_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
            .setMessage("Пропущен do");
    }
    block();
}

void Parser::print_statement()
{
    lexer.nextToken();
    while (true) {
        if (lexer.lookForToken().type == LITERAL) {
            lexer.nextToken();
        } else {
            add_expr();
        }

        if (lexer.lookForToken().compare(ONE_LIT_DELIM, COMMA)) {
            lexer.nextToken();
        } else {
            line_sep();
            break;
        }
    }
}

void Parser::read_statement()
{
    lexer.nextToken();
    Token tok = lexer.nextToken();
    if (tok.type == IDENTIFIER) {

    }

    if (lexer.lookForToken().compare(ONE_LIT_DELIM, LBRACKET)) {
        lexer.nextToken();
        tok = lexer.nextToken();
        if (tok.type != INTEGER && tok.type != IDENTIFIER) {
            throw SyntaxException(errs::UNEXPECTED_TOKEN)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
        }

        //логика

        if (!lexer.nextToken().compare(ONE_LIT_DELIM, RBRACKET)) {
            throw SyntaxException(errs::RBRACKET_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
        }
    }

    line_sep();
}

void Parser::assignment()
{

    if (lexer.nextToken().type == IDENTIFIER) {

    }

    if (lexer.nextToken().compare(ONE_LIT_DELIM, ASSIGN)) {
        add_expr();
    } else {
        throw SyntaxException(errs::OPERATOR_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
//            .setMessage("assignment");
    }
}


void Parser::or_expr()
{
    and_expr();
    Token tok = lexer.lookForToken();
    while (!followsOr(tok)) {
        tok = lexer.nextToken();
        if (tok.compare(TWO_LIT_DELIM, OR)) {
            and_expr();

        } else {
            throw SyntaxException(errs::OPERATOR_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
//                .setMessage("or");
        }
        tok = lexer.lookForToken();
    }
}

void Parser::and_expr()
{
    l_unary_expr();
    Token tok = lexer.lookForToken();
    while (!followsAnd(tok)) {
        tok = lexer.nextToken();
        if (tok.compare(TWO_LIT_DELIM, AND)) {
            l_unary_expr();

        } else {
            throw SyntaxException(errs::OPERATOR_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
//                .setMessage("and");
        }
        tok = lexer.lookForToken();
    }
}

void Parser::equation()
{
    relation();

    auto tok = lexer.lookForToken();
    if (!followsEquation(tok)) {
        tok = lexer.nextToken();
        if (tok.compare(TWO_LIT_DELIM, EQUALS)) {
            relation();


        } else if (tok.compare(TWO_LIT_DELIM, NOT_EQ)) {
            relation();


        } else {
            throw SyntaxException(errs::OPERATOR_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
//                .setMessage("eq");
        }

    }
}

void Parser::relation()
{
    add_expr();

    auto tok = lexer.lookForToken();
    bool bbb = followsRelation(tok);
    if (!followsRelation(tok)) {
        tok = lexer.nextToken();
        if (tok.compare(ONE_LIT_DELIM, MORE)) {
            add_expr();

        } else if (tok.compare(ONE_LIT_DELIM, LESS)) {
            add_expr();

        } else if (tok.compare(TWO_LIT_DELIM, LESS_OR_EQ)) {
            add_expr();

        } else if (tok.compare(TWO_LIT_DELIM, MORE_OR_EQ)) {
            add_expr();

        } else {
            throw SyntaxException(errs::OPERATOR_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
//                .setMessage("rel");
        }
    }
}

void Parser::add_expr()
{
    mul_expr();
    Token tok = lexer.lookForToken();
    while (!followsAdd(tok)) {
        tok = lexer.nextToken();
        if (tok.compare(ONE_LIT_DELIM, PLUS)) {
            mul_expr();

        } else if (tok.compare(ONE_LIT_DELIM, MINUS)) {
            mul_expr();

        } else {
            throw SyntaxException(errs::OPERATOR_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
//                .setMessage("add");
        }
        tok = lexer.lookForToken();
    }
}

void Parser::mul_expr()
{
    m_unary_expr();
    auto tok = lexer.lookForToken();
    while (!followsMul(tok)) {
        tok = lexer.nextToken();
        if (tok.compare(ONE_LIT_DELIM, DIV)) {
            m_primary_expr();

        } else if (tok.compare(ONE_LIT_DELIM, MULT)) {
            m_primary_expr();

        } else {
            throw SyntaxException(errs::OPERATOR_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
//                .setMessage("mul");
        }
        tok = lexer.lookForToken();
    }
}

void Parser::l_unary_expr()
{
    auto tok = lexer.lookForToken();
    if (tok.compare(ONE_LIT_DELIM, NOT)) {

    }
    
    l_primary_expr();
}

void Parser::m_unary_expr()
{
    auto tok = lexer.lookForToken();
    if (tok.compare(ONE_LIT_DELIM, MINUS)) {
        lexer.nextToken();
    }

    m_primary_expr();
}

void Parser::postfix_expr()
{
    auto tok = lexer.nextToken();
    /////////
    
    tok = lexer.lookForToken();
    if (tok.compare(ONE_LIT_DELIM, LBRACKET)) {
        lexer.nextToken();
        
        add_expr();
        
        if (!lexer.nextToken().compare(ONE_LIT_DELIM, RBRACKET)) {
            throw SyntaxException(errs::RBRACKET_MISSING)
                .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
        }
    }
    
}

void Parser::l_primary_expr()
{
    auto tok = lexer.nextToken();

    if (tok.compare(KEY_WORD, KEY_TRUE)) {

        return;
    }

    if (tok.compare(KEY_WORD, KEY_FALSE)) {

        return;
    }

    if (tok.compare(ONE_LIT_DELIM, OneLitDelim::EXPR_SEP)) {
        equation();
        if (lexer.nextToken().compare(ONE_LIT_DELIM, OneLitDelim::EXPR_SEP)) {

            return;
        }
        throw SyntaxException(errs::EXPR_SEP_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());

    }
    if (tok.compare(ONE_LIT_DELIM, OneLitDelim::LPAREN)) {
        or_expr();
        if (lexer.nextToken().compare(ONE_LIT_DELIM, OneLitDelim::RPAREN)) {

            return;
        }
        throw SyntaxException(errs::RPAREN_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
    }
    throw SyntaxException(errs::OPERAND_MISSING)
        .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
        .setMessage("Ожидалось true, false, переменная, '(' или '|'");
}


void Parser::m_primary_expr()
{
	auto tok = lexer.lookForToken();
	
	if (tok.type == IDENTIFIER) {
		postfix_expr();
        return;
    }
    
    tok = lexer.nextToken();
    if (tok.type == INTEGER) {

        return;
    }

    

    if (tok.compare(ONE_LIT_DELIM, OneLitDelim::LPAREN)) {
        add_expr();
        if (lexer.nextToken().compare(ONE_LIT_DELIM, OneLitDelim::RPAREN)) {

            return;
        }
        throw SyntaxException(errs::RPAREN_MISSING)
            .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition());
    }
    throw SyntaxException(errs::OPERAND_MISSING)
        .setLineAndPos(lexer.getLine(), lexer.getLastTokenPosition())
        .setMessage("Ожидалось число, переменная или '('");

}

bool Parser::followsOr(const Token &tok) const noexcept
{
    return (tok.type == ONE_LIT_DELIM && tok.id == RPAREN || tok.id == LINESEP || tok.id == COMMA);
}

bool Parser::followsAnd(const Token &tok) const noexcept
{
    return followsOr(tok) || tok.compare(TWO_LIT_DELIM, OR);
}

bool Parser::followsEquation(const Token &tok) const noexcept
{
    return followsAnd(tok) || tok.type == TWO_LIT_DELIM && tok.id == AND || tok.compare(ONE_LIT_DELIM, EXPR_SEP);
}

bool Parser::followsRelation(const Token &tok) const noexcept
{
    return followsEquation(tok) || tok.type == TWO_LIT_DELIM && (tok.id == EQUALS || tok.id == NOT_EQ);
}

bool Parser::followsAdd(const Token &tok) const noexcept
{
    return followsRelation(tok) || (tok.type == ONE_LIT_DELIM && tok.id == MORE || tok.id == LESS) ||
        (tok.type == TWO_LIT_DELIM && tok.id == MORE_OR_EQ || tok.id == LESS_OR_EQ);
}

bool Parser::followsMul(const Token &tok) const noexcept
{
    return followsAdd(tok) || tok.type == ONE_LIT_DELIM && tok.id == PLUS || tok.id == MINUS;
}

bool Parser::followsPostfixExpr(const Token &tok) const noexcept
{
    return followsMul(tok);
}
