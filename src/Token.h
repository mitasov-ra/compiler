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

        bool operator== (const Token& t2) const noexcept {
            return id == t2.id && type == t2.type;
        }

        bool operator!= (const Token t2) const noexcept {
            return id != t2.id || type != t2.type;
        }

        Token(int t, int p) :
                type(t),
                id(p) {}
    };
}
#endif //COMPILER_TOKEN_H


