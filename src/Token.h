//
// Created by ROMAN on 16.10.2017.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H


#include <string>
namespace compiler {

    struct Token {
        int type;
        int id;

        bool operator== (Token t2) {
            return id == t2.id && type == t2.type;
        }

        bool operator!= (Token t2) {
            return id != t2.id || type != t2.type;
        }

        Token(int t, int p) :
                type(t),
                id(p) {}
    };
}
#endif //COMPILER_TOKEN_H


