//
// Created by ROMAN on 03.10.2017.
//

#ifndef COMPILER_SYNTAXEXCEPTION_H
#define COMPILER_SYNTAXEXCEPTION_H

#include <string>
namespace compiler {
    class SyntaxException {
    public:
        enum class Errors {
            UNKNOWN_TOKEN,

            OPERAND_MISSING,
            END_OF_LINE_MISSING,
            OPERATOR_MISSING,
        };
    private:

        int position;
        int line;
        std::string errorMessage;
        Errors errorId;
    public:
        explicit SyntaxException(Errors err);
        SyntaxException setMessage(const std::string& message);
        SyntaxException setLineAndPos(int line, int pos);
        std::string getErrorMessage() const;
        void printError();
    };
}


#endif //COMPILER_SYNTAXEXCEPTION_H
