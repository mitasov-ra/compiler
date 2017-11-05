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
        void math_expression();

        void math_mult();

        void math_operand();


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
