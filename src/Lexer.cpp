#include "compiler.h"
#include "Lexer.h"
#include "SyntaxException.h"

//#include <iostream>

using namespace compiler;



/*
 * Инициализация набора ключевых слов
 */
const std::unordered_map<std::string, Token> Lexer::keyWordTokens = {
        {"var",     {KEY_WORD, KEY_VAR}},
        {"if",      {KEY_WORD, KEY_IF}},
        {"then",    {KEY_WORD, KEY_THEN}},
        {"else",    {KEY_WORD, KEY_ELSE}},
        {"print",   {KEY_WORD, KEY_PRINT}},
        {"program", {KEY_WORD, KEY_PROGRAM}},
        {"true",    {KEY_WORD, KEY_TRUE}},
        {"false",   {KEY_WORD, KEY_FALSE}},
        {"array",   {KEY_WORD, KEY_ARRAY}},
        {"int",     {KEY_WORD, KEY_INT}},
        {"string",  {KEY_WORD, KEY_STRING}},
};

/**
* Функция возвращает указатель на
* следующую по очереди лексему
*
* @return std::shared_ptr<Token>
*/
Token Lexer::nextToken() {
    if (foundToken != nullptr) {
        auto retTok = foundToken;
        foundToken = nullptr;
        return *retTok;
    }

    enum states {
        start,
        numeric,
        identifier,
        div,
        plus,
        min,
        pow,
        mul,
        comment,
        comment_end,
        lineComment,
        error,
        eqOrAssign,
        less,
        more,
        st_not,
        st_string,
        st_and,
        st_or,
    };
    last_position = position;
    std::string word;
    states state = start;
    while (true) {
        if (position == buffer.length()) {
            loadLine();
        }
        const char &ch = buffer[position];
        switch (state) {
            case start:
                if (isWhitespace(ch)) {
                    position++;
                    last_position++;
                    break;
                } else if (isNumeric(ch)) {
                    word += ch;
                    state = numeric;
                    position++;
                    break;
                } else if (isLetter(ch)) {
                    word += ch;
                    state = identifier;
                    position++;
                    break;
                }

                switch (ch) {
                    case Lexer::ENDING_CHAR:
                        return {ONE_LIT_DELIM, INPUT_END};
                    case ';':
                        position++;
                        return {ONE_LIT_DELIM, LINESEP};
                    case ',':
                        position++;
                        return {ONE_LIT_DELIM, COMMA};
                    case '"':
                        position++;
                        state = st_string;
                        break;
                    case '(':
                        position++;
                        return {ONE_LIT_DELIM, LPAREN};
                    case ')':
                        position++;
                        return {ONE_LIT_DELIM, RPAREN};
                    case '[':
                        position++;
                        return {ONE_LIT_DELIM, LBRACKET};
                    case ']':
                        position++;
                        return {ONE_LIT_DELIM, RBRACKET};
                    case '{':
                        position++;
                        return {ONE_LIT_DELIM, LBRACE};
                    case '}':
                        position++;
                        return {ONE_LIT_DELIM, RBRACE};

                    case '*':
                        position++;
                        state = mul;
                        break;
                    case '+':
                        position++;
                        state = plus;
                        break;
                    case '-':
                        position++;
                        state = min;
                        break;
                    case '/':
                        position++;
                        state = div;
                        break;
                    case '=':
                        position++;
                        state = eqOrAssign;
                        break;
//                    case '^':
//                        position++;
//                        return {ONE_LIT_DELIM, POW};
                    case '<':
                        position++;
                        state = less;
                        break;
                    case '>':
                        position++;
                        state = more;
                        break;
                    case '!':
                        position++;
                        state = st_not;
                        break;
                    case '&':
                        position++;
                        state = st_and;
                        break;
                    case '|':
                        position++;
                        state = st_or;
                        break;
                    default:
                        state = error;
                }

                break;

            case numeric:
                if (isNumeric(ch)) {
                    word += ch;
                    position++;
                } else if (isWhitespace(ch) || isOperator(ch) || ch == ENDING_CHAR || ch == ';') {
                    return {INTEGER, tables->insertInteger(word)};
                } else {
                    state = error;
                }
                break;

            case identifier:
                if (isLetter(ch) || isNumeric(ch)) {
                    word += ch;
                    position++;
                } else if (isWhitespace(ch) || isOperator(ch) || ch == ';' || ch == ENDING_CHAR) {

                    {
                        auto it = keyWordTokens.find(word);
                        if (it != keyWordTokens.end()) {
                            return it->second;
                        }
                    }
                    return {IDENTIFIER, tables->insertIdentifier(word)};
                } else {
                    state = error;
                }
                break;

            case st_string:
                if (ch == '\n') {
                    state = error;
                    break;
                }
                if (ch != '"') {
                    word += ch;
                    position++;
                } else {
                    position++;
                    return {LITERAL, tables->insertString(word)};
                }
                break;

            case div:
                if (ch == '*') {
                    position++;
                    state = comment;
                } else if (ch == '/') {
                    position++;
                    state = lineComment;
                } else if (ch == '=') {
                    position++;
                    return {TWO_LIT_DELIM, DIV_ASSIGN};
                } else {
                    return {ONE_LIT_DELIM, DIV};
                }
                break;

            case comment:
                if (ch == '*') {
                    position++;
                    state = comment_end;
                } else if (ch == ENDING_CHAR) {
                    state = start;
                } else {
                    position++;
                }
                break;

            case comment_end:
                if (ch == '/') {
                    position++;
                    state = start;
                } else {
                    position++;
                    state = comment;
                }
                break;

            case lineComment:
                if (ch == '\n' || ch == ENDING_CHAR) {
                    state = start;
                } else {
                    position++;
                }
                break;

            case plus:
                if (ch == '=') {
                    position++;
                    return {TWO_LIT_DELIM, PLUS_ASSIGN};
                }
                if (ch == '+') {
                    position++;
                    return {TWO_LIT_DELIM, INCR};
                }
                return {ONE_LIT_DELIM, PLUS};

            case min:
                if (ch == '=') {
                    position++;
                    return {TWO_LIT_DELIM, MINUS_ASSIGN};
                }
                if (ch == '+') {
                    position++;
                    return {TWO_LIT_DELIM, DECR};
                }
                return {ONE_LIT_DELIM, MINUS};

            case mul:
                if (ch == '=') {
                    position++;
                    return {TWO_LIT_DELIM, MULT_ASSIGN};
                }
                return {ONE_LIT_DELIM, MULT};

            case eqOrAssign:
                if (ch == '=') {
                    position++;
                    return {TWO_LIT_DELIM, EQUALS};
                }
                return {ONE_LIT_DELIM, ASSIGN};

            case more:
                if (ch == '=') {
                    position++;
                    return {TWO_LIT_DELIM, MORE_OR_EQ};
                }
                return {ONE_LIT_DELIM, MORE};

            case less:
                if (ch == '=') {
                    position++;
                    return {TWO_LIT_DELIM, LESS_OR_EQ};
                }
                return {ONE_LIT_DELIM, LESS};

            case st_not:
                if (ch == '=') {
                    position++;
                    return {TWO_LIT_DELIM, NOT_EQ};
                }
                return {ONE_LIT_DELIM, NOT};

            case st_and:
                if (ch == '&') {
                    position++;
                    return Token{TWO_LIT_DELIM, AND};
                }
                state = error;
                break;

            case st_or:
                if (ch == '|') {
                    position++;
                    return Token{TWO_LIT_DELIM, OR};
                }

            case error:
                while (!(isWhitespace(buffer[position]) || buffer[position] == INPUT_END ||
                         isOperator(buffer[position]))) //|| isOperator(buffer[position]))
                    position++;
                throw SyntaxException(SyntaxException::Errors::UNKNOWN_TOKEN).setLineAndPos(line, last_position);
        }
    }
}

// /**
// * Инициализация полей класса входящей строкой
// *
// * @param str
// */
//void Lexer::initString(const std::string & str) {
//    buffer = str;
//    position = 0;
//    buffer += ENDING_CHAR;
//}
//
//void Lexer::initStream(std::istream& fin) {
//    this->fin = &fin;
//    loadLine();
//}

void Lexer::loadLine() {
    std::getline(*fin, buffer);
    buffer += '\n';
    position = 0;
    last_position = position;
    line++;
    if (fin->eof())
        buffer += ENDING_CHAR;
}

// /**
// * Функция проходит по строке и возвращает
// * последовательность токенов
// *
// * @param str
// * @param vect
// */
//void Lexer::scan(std::vector<std::shared_ptr<Token>>& vect) {
//    std::shared_ptr<Token> tok = nullptr;
//    do {
//        try {
//            tok = nextToken();
// //            std::cout << "tok = " << tok->getTokenType() << ' ' << tok->getToken() << std::endl;
//        } catch (SyntaxException &e) {
// //            tok = TOK_ERROR;
// //           std::cout << "error\n";
//        }
//        vect.push_back(tok);
//    } while (!(tok->getTokenType() == ONE_LIT_DELIM && tok->getToken() == INPUT_END));
//}

Lexer::Lexer(const std::string &str, const std::shared_ptr<TokenTables>& tables) :
    buffer (str),
    position (0),
    line(1),
    tables (tables)
{
    buffer += ENDING_CHAR;
}

Lexer::Lexer(std::istream &fin, const std::shared_ptr<TokenTables>& tables) :
    fin (&fin),
    tables (tables),
    line(0)
{
    loadLine();
}



Token Lexer::lookForToken() {
    if (foundToken != nullptr)
        return *foundToken;

    foundToken = std::make_shared<Token>(nextToken());
    return *foundToken;
}

int Lexer::getPosition() const {
    return position;
}

int Lexer::getLastTokenPosition() const {
    return last_position + 1;
}

int Lexer::getLine() const {
    return line;
}



//std::string Lexer::getToken(Lexer::TokenType type, int index) const
//{
//    std::string id;
//
//    switch (type)
//    {
//        case NUMBER:
//            id = numberTokens[index];
//            break;
//        case IDENTIFIER:
//            id = identifierTokens[index];
//            break;
//        case KEY_WORD:
//            id = keyWordTokens[index];
//            break;
//        case BREAKER:
//            id = breakerTokens[index];
//            break;
//    }
//
//
//    return id;
//}
//
//int Lexer::addToken(Lexer::TokenType type, const std::string& s) {
//    std::vector<std::string> *table;
//
//    switch (type)
//    {
//        case NUMBER:
//            table = &numberTokens;
//            break;
//        case IDENTIFIER:
//
//            table = &identifierTokens;
//            break;
//        case BREAKER:
//            table = &breakerTokens;
//            break;
//    }
//
//    auto it = std::find(table->begin(), table->end(), s);
//    if ( it != table->end())
//    {
//        //std::cout << "  --- found in table\n";
//        return it - table->begin();
//    }
//    //std::cout << "  --- not found, making new id\n";
//    table->push_back(s);
//
//    return table->size() - 1;
//}

