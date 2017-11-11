#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H
#include <iosfwd>
#include <utility>

#include "Lexer.h"
#include "TokenTables.h"

namespace compiler {
    class Parser {
    private:
        Lexer lexer;
        std::shared_ptr<TokenTables> tables;

        bool followsAssigment(const Token& t) const noexcept;

        bool followsOr(const Token& t) const noexcept;

        bool followsAnd(const Token& t) const noexcept;

        bool followsEquation(const Token& t) const noexcept;

        bool followsRelation(const Token& t) const noexcept;

        bool followsAdd(const Token& t) const noexcept;

        bool followsMul(const Token& t) const noexcept;

        bool followsPostfixExpr(const Token &tok) const noexcept;

        void line_sep();

        void program();

        void var();
        
        void type();
        
        void array_type();

        void block();
        
        void statement();

        void if_statement();

        void else_statement();
        
        void while_statement();

        void print_statement();

        void assignment();

        void or_expr();

        void and_expr();

        void equation();

        void relation();

        void add_expr();

        void mul_expr();

        void unary_expr();

        void postfix_expr();

        void primary_expr();

    public:
        void parse(const std::string &);

        Parser(const std::string &str, const std::shared_ptr<TokenTables> &tables) :
                tables(tables),
                lexer(str, tables)
        {}

        Parser(std::istream &sin, const std::shared_ptr<TokenTables> &tables) :
                tables(tables),
                lexer(sin, tables)
        {}


    };
}
#endif
