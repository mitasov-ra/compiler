//
// Created by ROMAN on 16.10.2017.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H


#include <string>

namespace compiler
{

    struct Token
    {
        int type;
        int id;

        bool operator==(const Token &t2) const noexcept
        {
            return id == t2.id && type == t2.type;
        }

        bool operator!=(const Token t2) const noexcept
        {
            return id != t2.id || type != t2.type;
        }

        inline bool compare(const int t, const int i) const noexcept
        {
            return id == i && type == t;
        }

        Token(int t, int i) :
            type(t),
            id(i) {}
    };
}
#endif //COMPILER_TOKEN_H


