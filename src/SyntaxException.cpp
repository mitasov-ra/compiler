//
// Created by ROMAN on 03.10.2017.
//

#include <iostream>
#include "SyntaxException.h"

using namespace compiler;

std::string SyntaxException::getErrorMessage() const {
    return errorMessage;
}

SyntaxException::SyntaxException(SyntaxException::Errors err) :
    errorId(err),
    line(-1),
    position(-1)
{}

SyntaxException SyntaxException::setMessage(const std::string &message) {
    errorMessage = message;
    return *this;
}

SyntaxException SyntaxException::setLineAndPos(int line, int pos) {
    this->line = line;
    this->position = pos;
    return *this;
}

void SyntaxException::printError() {
    std::cout << "Синтаксическая ошибка";
    if (line != -1 && position != -1)
        std::cout << " (" << line << ", " << position << ")";
    std::cout << ": ";
    switch (errorId) {
        case Errors::UNKNOWN_TOKEN:
            std::cout << "Неизвестный токен\n";
            break;

        case Errors::END_OF_LINE_MISSING:
            std::cout << "Нет конца строки\n";
            break;

        case Errors::OPERATOR_MISSING:
            std::cout << "Требуется оператор или конец строки\n";
            break;

        case Errors::OPERAND_MISSING:
            std::cout << "Ожидалось число или идентификатор\n";
            break;
    }

    if (!errorMessage.empty()) {
        std::cout << errorMessage << std::endl;
    }

}
