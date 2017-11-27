//
// Created by ROMAN on 03.10.2017.
//

#ifndef COMPILER_SYNTAXEXCEPTION_H
#define COMPILER_SYNTAXEXCEPTION_H

#include <string>

namespace compiler
{
    class SyntaxException
    {
    public:
        enum class Errors
        {
            UNKNOWN_TOKEN,

            UNEXPECTED_TOKEN,
            LINESEP_MISSING,
            CODE_AFTER_END,
            KEYWORD_MISSING,
            IDENTIFIER_MISSING,
            LBRACE_MISSING,
            RBRACE_MISSING,
            LPAREN_MISSING,
            RPAREN_MISSING,
            LBRACKET_MISSING,
            RBRACKET_MISSING,
            EXPR_SEP_MISSING,

            OPERAND_MISSING,
            OPERATOR_MISSING,
        };
    private:

        int position;
        int line;
        std::string errorMessage;
        Errors errorId;
    public:
        explicit SyntaxException(Errors err);

        SyntaxException setMessage(const std::string &message);

        SyntaxException setLineAndPos(int line, int pos);

        std::string getErrorMessage() const;

        void printError();
    };
}


#endif //COMPILER_SYNTAXEXCEPTION_H
