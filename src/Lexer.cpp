#include "compiler.h"
#include "Lexer.h"
#include "SyntaxException.h"

#include <memory>
//#include <iostream>

using namespace compiler;

/*
 * Инициализация константных токенов
 * при инициализации указвается тип
 */
    const Token Lexer::TOK_PLUS(ONE_LIT_DELIM, PLUS);
    const Token Lexer::TOK_MINUS(ONE_LIT_DELIM, MINUS);
    const Token Lexer::TOK_MULT(ONE_LIT_DELIM, MULT);
    const Token Lexer::TOK_DIV(ONE_LIT_DELIM, DIV);
    const Token Lexer::TOK_POW(ONE_LIT_DELIM, POW);

    const Token Lexer::TOK_ASSIGN(ONE_LIT_DELIM, ASSIGN);

    const Token Lexer::TOK_LPAREN(ONE_LIT_DELIM, LPAREN);
    const Token Lexer::TOK_RPAREN(ONE_LIT_DELIM, RPAREN);
    const Token Lexer::TOK_LBRACE(ONE_LIT_DELIM, LBRACE);
    const Token Lexer::TOK_RBRACE(ONE_LIT_DELIM, RBRACE);
    const Token Lexer::TOK_LBRACKET(ONE_LIT_DELIM, LBRACKET);
    const Token Lexer::TOK_RBRACKET(ONE_LIT_DELIM, RBRACKET);

    const Token Lexer::TOK_MORE(ONE_LIT_DELIM, MORE);
    const Token Lexer::TOK_LESS(ONE_LIT_DELIM, LESS);
    const Token Lexer::TOK_NOT(ONE_LIT_DELIM, NOT);

    const Token Lexer::TOK_INCR(TWO_LIT_DELIM, INCR);
    const Token Lexer::TOK_DECR(TWO_LIT_DELIM, DECR);

    const Token Lexer::TOK_PLUS_ASSIGN(TWO_LIT_DELIM, PLUS_ASSIGN);
    const Token Lexer::TOK_MINUS_ASSIGN(TWO_LIT_DELIM, MINUS_ASSIGN);
    const Token Lexer::TOK_MULT_ASSIGN(TWO_LIT_DELIM, MULT_ASSIGN);
    const Token Lexer::TOK_DIV_ASSIGN(TWO_LIT_DELIM, DIV_ASSIGN);

    const Token Lexer::TOK_EQUALS(TWO_LIT_DELIM, EQUALS);
    const Token Lexer::TOK_MORE_OR_EQ(TWO_LIT_DELIM, MORE_OR_EQ);
    const Token Lexer::TOK_LESS_OR_EQ(TWO_LIT_DELIM, LESS_OR_EQ);
    const Token Lexer::TOK_NOT_EQ(TWO_LIT_DELIM, NOT_EQ);

    const Token Lexer::TOK_LINE_END(ONE_LIT_DELIM, LINE_DELIM);
    const Token Lexer::TOK_INPUT_END(ONE_LIT_DELIM, INPUT_END);
    const Token Lexer::TOK_COMMA(ONE_LIT_DELIM, COMMA);

    const Token Lexer::TOK_KEYWORD_PROGRAM(KEY_WORD, KEY_PROGRAM);
    const Token Lexer::TOK_KEYWORD_VAR(KEY_WORD, KEY_VAR);
    const Token Lexer::TOK_KEYWORD_IF(KEY_WORD, KEY_IF);
    const Token Lexer::TOK_KEYWORD_PRINT(KEY_WORD, KEY_PRINT);






//const std::shared_ptr<Token> Lexer::TOK_ERROR           (ERROR));


/*
 * Инициализация набора ключевых слов
 */
const std::unordered_map<std::string, Token> Lexer::keyWordTokens = {
        {"var",     Lexer::TOK_KEYWORD_VAR},
        {"if",      Lexer::TOK_KEYWORD_IF},
        {"print",   Lexer::TOK_KEYWORD_PRINT},
        {"program", Lexer::TOK_KEYWORD_PROGRAM},
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
                        return TOK_INPUT_END;
                    case ';':
                        position++;
                        return TOK_LINE_END;
                    case ',':
                        position++;
                        return TOK_COMMA;
                    case '"':
                        position++;
                        state = st_string;
                        break;
                    case '(':
                        position++;
                        return TOK_LPAREN;
                    case ')':
                        position++;
                        return TOK_RPAREN;
                    case '[':
                        position++;
                        return TOK_LBRACKET;
                    case ']':
                        position++;
                        return TOK_RBRACKET;
                    case '{':
                        position++;
                        return TOK_LBRACE;
                    case '}':
                        position++;
                        return TOK_RBRACE;

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
                    case '^':
                        position++;
                        return TOK_POW;
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
                    default:
                        state = error;
                }

                break;

            case numeric:
                if (isNumeric(ch)) {
                    word += ch;
                    position++;
                } else if (isWhitespace(ch) || isOperator(ch) || ch == ENDING_CHAR || ch == ';') {
                    return Token(INTEGER, tables->insertInteger(word));
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
                    return Token(IDENTIFIER, tables->insertIdentifier(word));
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
                    return Token(LITERAL, tables->insertString(word));
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
                    return TOK_DIV_ASSIGN;
                } else {
                    return TOK_DIV;
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
                    return TOK_PLUS_ASSIGN;
                }
                if (ch == '+') {
                    position++;
                    return TOK_INCR;
                }
                return TOK_PLUS;

            case min:
                if (ch == '=') {
                    position++;
                    return TOK_MINUS_ASSIGN;
                }
                if (ch == '+') {
                    position++;
                    return TOK_DECR;
                }
                return TOK_MINUS;

            case mul:
                if (ch == '=') {
                    position++;
                    return TOK_MULT_ASSIGN;
                }
                return TOK_MULT;

            case eqOrAssign:
                if (ch == '=') {
                    position++;
                    return TOK_EQUALS;
                }
                return TOK_ASSIGN;

            case more:
                if (ch == '=') {
                    position++;
                    return TOK_MORE_OR_EQ;
                }
                return TOK_MORE;

            case less:
                if (ch == '=') {
                    position++;
                    return TOK_LESS_OR_EQ;
                }
                return TOK_LESS;

            case st_not:
                if (ch == '=') {
                    position++;
                    return TOK_NOT_EQ;
                }
                return TOK_NOT;

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
    return last_position;
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

