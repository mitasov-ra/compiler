#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include <istream>
//#include <vector>
#include <memory>
#include <unordered_map>
#include "Token.h"
#include "TokenTables.h"

namespace compiler {
    class Lexer {
    private:
        /*
         * Буферная строка
         */
        std::string buffer;
        int position; //текушая позиция в буферной строке

        int line;
        int last_position;

        static constexpr char ENDING_CHAR = '$'; // символ конца файла

        std::istream *fin;

        /*
         * Набор строк с ключевыми словами,
         * используется для быстрого поиска
         */
        static const std::unordered_map<std::string, Token> keyWordTokens;

        std::shared_ptr<Token> foundToken;

        void loadLine();

        inline bool isNumeric(char ch) {
            return ch >= '0' && ch <= '9';
        }

        inline bool isLetter(char ch) {
            return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
        }

        inline bool isWhitespace(char ch) {
            return ((std::string) "\r\n\t ").find(ch) != -1;
        }

        inline bool isOperator(char ch) {
            return ((std::string) "=+-*/^()[]{}<>").find(ch) != -1;
        }

    public:

//    /*
//     * Перечисление всех возможных типов токенов
//     */
//    enum TokenType {
//		KEY_WORD,
//		ONE_LIT_DELIM,
//		TWO_LIT_DELIM,
//		INTEGER,
//        IDENTIFIER,
//        LITERAL,
//	};
//
//	enum OneLitDelim {
//		INPUT_END,
//		LINE_DELIM,
//		PLUS,
//		MINUS,
//        MULT,
//        DIV,
//        POW,
//        ASSIGN,
//        LPAREN,
//        RPAREN,
//        LBRACKET,
//        RBRACKET,
//        LBRACE,
//        RBRACE,
//        MORE,
//        LESS,
//        NOT,
//        COMMA,
//	};
//
//	enum TwoLitDelim {
//		PLUS_ASSIGN,
//        MINUS_ASSIGN,
//        DIV_ASSIGN,
//        MULT_ASSIGN,
//        INCR,
//        DECR,
//        EQUALS,
//        MORE_OR_EQ,
//        LESS_OR_EQ,
//        NOT_EQ,
//	};
//
//	enum KeyWord {
//        KEY_PROGRAM,
//    //    ERROR,
//        KEY_VAR,
//        KEY_IF,
//        KEY_PRINT,
//    };
//
//    std::vector<std::string> identifiers;
//    std::vector<int> integers;
//    std::vector<std::string> literals;

        /*
         * Константный набор токенов операторов
         */
        static const Token TOK_PLUS;
        static const Token TOK_MINUS;
        static const Token TOK_MULT;
        static const Token TOK_DIV;
        static const Token TOK_POW;
        static const Token TOK_PLUS_ASSIGN;
        static const Token TOK_MINUS_ASSIGN;
        static const Token TOK_MULT_ASSIGN;
        static const Token TOK_DIV_ASSIGN;
        static const Token TOK_ASSIGN;
        static const Token TOK_INCR;
        static const Token TOK_DECR;
        static const Token TOK_LPAREN;
        static const Token TOK_RPAREN;
        static const Token TOK_LBRACE;
        static const Token TOK_RBRACE;
        static const Token TOK_LBRACKET;
        static const Token TOK_RBRACKET;
        static const Token TOK_MORE;
        static const Token TOK_LESS;
        static const Token TOK_NOT;
        static const Token TOK_EQUALS;
        static const Token TOK_MORE_OR_EQ;
        static const Token TOK_LESS_OR_EQ;
        static const Token TOK_NOT_EQ;
        static const Token TOK_COMMA;


        /*
         * Константный набор токенов - специальных символов
         * и ключевых слов
         */
        static const Token TOK_LINE_END;
        static const Token TOK_INPUT_END;
        static const Token TOK_KEYWORD_VAR;
        static const Token TOK_KEYWORD_IF;
        static const Token TOK_KEYWORD_PRINT;
        static const Token TOK_KEYWORD_PROGRAM;
  //    static const Token TOK_ERROR;

        std::shared_ptr<TokenTables> tables = nullptr;

        Token nextToken();

        Token lookForToken();

        int getPosition() const;
        int getLastTokenPosition() const;
        int getLine() const;
//    void scan(std::vector<std::shared_ptr<Token>>& vect);


        Lexer(const std::string &str, const std::shared_ptr<TokenTables>& tables);

        Lexer(std::istream &fin, const std::shared_ptr<TokenTables>& tables);

//    void initString(const std::string &str);
//    void initStream(std::istream &fin);
    };
}


#endif // COMPILER_LEXER_H
