#include <iostream>
#include <conio.h>
#include <fstream>

#include "compiler.h"

using namespace std;
using namespace compiler;

string tokenType[] = {
    "KEY_WORD",
    "ONE_LIT_DELIM",
    "TWO_LIT_DELIM",
    "INTEGER",
    "IDENTIFIER",
    "LITERAL",
};

string oneLitDelim[] = {
    "INPUT_END",
    "LINESEP",
    "PLUS",
    "MINUS",
    "MULT",
    "DIV",
    //"POW",
    "ASSIGN",
    "LPAREN",
    "RPAREN",
    "LBRACKET",
    "RBRACKET",
    "LBRACE",
    "RBRACE",
    "MORE",
    "LESS",
    "NOT",
    "COMMA",
};

string twoLitDelim[] = {
    "PLUS_ASSIGN",
    "MINUS_ASSIGN",
    "DIV_ASSIGN",
    "MULT_ASSIGN",
    "INCR",
    "DECR",
    "EQUALS",
    "MORE_OR_EQ",
    "LESS_OR_EQ",
    "NOT_EQ",
};

string keyWord[] = {
        "KEY_PROGRAM",
        "KEY_VAR",
        "KEY_IF",
        "KEY_ELSE",
        "KEY_THEN",
        "KEY_PRINT",
        "KEY_TRUE",
        "KEY_FALSE",
        "KEY_ARRAY",
        "KEY_INT",
        "KEY_STRING",
};

int main() {
    setlocale(LC_ALL, "rus");

    ifstream fin;
	fin.open("main.txt");

    if (!fin) {
        cout << "File does not exists\n";
        getch();
        return 0;
    }

    auto tables = make_shared<TokenTables>();

    auto lex = Lexer(fin, tables);

    std::shared_ptr<Token> tok = nullptr;
    while(true) {
        try {
            tok = make_shared<Token>(lex.nextToken());

//            std::cout << "tok = " << tok->type << ' ' << tok->id << std::endl;
            std::cout << tokenType[tok->type];
            switch (tok->type) {
                case ONE_LIT_DELIM:
                    std::cout << '\t' << oneLitDelim[tok->id];
                    if (tok->type == ONE_LIT_DELIM && tok->id == INPUT_END)
                       goto while_end;
                    break;
                case TWO_LIT_DELIM:
                    std::cout << '\t' << twoLitDelim[tok->id];
                    break;
                case KEY_WORD:
                    std::cout << '\t' << keyWord[tok->id];
                    break;
                case INTEGER:
                    std::cout << "\t\t" << tok->id << "\t`" << tables->integers[tok->id] << '`';
                    break;
                case IDENTIFIER:
                    std::cout << '\t' << tok->id << "\t`" << tables->identifiers[tok->id] << '`';
                    break;
                case LITERAL:
                  std::cout << "\t\t" << tok->id << "\t\"" << tables->literals[tok->id] << '"';
                  break;
            }
        } catch (SyntaxException &e) {
            e.printError();
        }
        cout << std::endl;
    }
    while_end:

    cout << std::endl;
    cout << std::endl;

    fin.close();
    fin.open("main.txt");

//    lex = Lexer(fin);
//    tok = lex.lookForToken();
//    switch (tok->type) {
//        case ONE_LIT_DELIM:
//            std::cout << '\t' << oneLitDelim[tok->id];
//            break;
//        case TWO_LIT_DELIM:
//            std::cout << '\t' << twoLitDelim[tok->id];
//            break;
//        case KEY_WORD:
//            std::cout << '\t' << keyWord[tok->id];
//            break;
//        case INTEGER:
//            std::cout << "\t\t" << tok->id << "\t`" << lex.integers[tok->id] << '`';
//            break;
//        case IDENTIFIER:
//            std::cout << '\t' << tok->id << "\t`" << lex.identifiers[tok->id] << '`';
//            break;
//        case LITERAL:
//            std::cout << "\t\t" << tok->id << "\t\"" << lex.literals[tok->id] << '"';
//            break;
//    }
//    cout << std::endl;
//
//    tok = lex.lookForToken();
//    switch (tok->type) {
//        case ONE_LIT_DELIM:
//            std::cout << '\t' << oneLitDelim[tok->id];
//            break;
//        case TWO_LIT_DELIM:
//            std::cout << '\t' << twoLitDelim[tok->id];
//            break;
//        case KEY_WORD:
//            std::cout << '\t' << keyWord[tok->id];
//            break;
//        case INTEGER:
//            std::cout << "\t\t" << tok->id << "\t`" << lex.integers[tok->id] << '`';
//            break;
//        case IDENTIFIER:
//            std::cout << '\t' << tok->id << "\t`" << lex.identifiers[tok->id] << '`';
//            break;
//        case LITERAL:
//            std::cout << "\t\t" << tok->id << "\t\"" << lex.literals[tok->id] << '"';
//            break;
//    }
//    cout << std::endl;
//
//    tok = lex.nextToken();
//    switch (tok->type) {
//        case ONE_LIT_DELIM:
//            std::cout << '\t' << oneLitDelim[tok->id];
//            break;
//        case TWO_LIT_DELIM:
//            std::cout << '\t' << twoLitDelim[tok->id];
//            break;
//        case KEY_WORD:
//            std::cout << '\t' << keyWord[tok->id];
//            break;
//        case INTEGER:
//            std::cout << "\t\t" << tok->id << "\t`" << lex.integers[tok->id] << '`';
//            break;
//        case IDENTIFIER:
//            std::cout << '\t' << tok->id << "\t`" << lex.identifiers[tok->id] << '`';
//            break;
//        case LITERAL:
//            std::cout << "\t\t" << tok->id << "\t\"" << lex.literals[tok->id] << '"';
//            break;
//    }
//    cout << std::endl;
//
//    tok = lex.lookForToken();
//    switch (tok->type) {
//        case ONE_LIT_DELIM:
//            std::cout << '\t' << oneLitDelim[tok->id];
//            break;
//        case TWO_LIT_DELIM:
//            std::cout << '\t' << twoLitDelim[tok->id];
//            break;
//        case KEY_WORD:
//            std::cout << '\t' << keyWord[tok->id];
//            break;
//        case INTEGER:
//            std::cout << "\t\t" << tok->id << "\t`" << lex.integers[tok->id] << '`';
//            break;
//        case IDENTIFIER:
//            std::cout << '\t' << tok->id << "\t`" << lex.identifiers[tok->id] << '`';
//            break;
//        case LITERAL:
//            std::cout << "\t\t" << tok->id << "\t\"" << lex.literals[tok->id] << '"';
//            break;
//    }
//    cout << std::endl;

    tables.reset();
    tables = make_shared<TokenTables>();
    Parser parser = Parser(fin, tables);
    try {
        parser.parse("file1");
        cout << "Успешно!\n";
    } catch (SyntaxException &e) {
        e.printError();
    }
    
    
    getch();
    
    return 0;
}
