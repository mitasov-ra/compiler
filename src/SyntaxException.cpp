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

        case Errors::LINESEP_MISSING:
            std::cout << "Отсутствует конец строки (';')\n";
            break;

        case Errors::OPERATOR_MISSING:
            std::cout << "Требуется оператор или конец строки\n";
            break;

        case Errors::OPERAND_MISSING:
            std::cout << "Отсутствует операнд\n";
            break;

        case Errors::CODE_AFTER_END:
            std::cout << "После конца программы не должно ничего быть\n";
            break;

        case Errors::KEYWORD_MISSING:
            std::cout << "Отсутствует ключевое слово\n";
            break;

        case Errors::IDENTIFIER_MISSING:
            std::cout << "Отсутствует идентификатор\n";
            break;

        case Errors::UNEXPECTED_TOKEN:
            std::cout << "Неожиданный символ\n";
            break;

        case Errors::LBRACE_MISSING:
            std::cout << "Отсутствует открывающая фигурная скобка\n";
            break;

        case Errors::RBRACE_MISSING:
            std::cout << "Отсутствует закрывающая фигурная скобка\n";
            break;

        case Errors::LPAREN_MISSING:
            std::cout << "Отсутствует открывающая скобка\n";
            break;

        case Errors::RPAREN_MISSING:
            std::cout << "Отсутствует закрывающая скобка\n";
            break;

        case Errors::LBRACKET_MISSING:
            std::cout << "Отсутствует открывающая квадратная скобка\n";
            break;

        case Errors::RBRACKET_MISSING:
            std::cout << "Отсутствует закрывающая квадратная скобка\n";
            break;

        case Errors::EXPR_SEP_MISSING:
            std::cout << "Отсутствует '|'\n";
            break;
    }

    if (!errorMessage.empty()) {
        std::cout << errorMessage << std::endl;
    }

}
