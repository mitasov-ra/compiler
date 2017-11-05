//
// Created by ROMAN on 16.10.2017.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H


#include <string>
namespace compiler {

    class Token {
    private:

//    std::string* name;
//    std::string* value;
    public:
        int type;
        int id;

        Token(int t, int p) :
//      name(nullptr),
//      value(nullptr),
                type(t),
                id(p) {}
//    void setValue(const std::string &);
//    void setName(const std::string &);
//    std::string getValue() const;
//    std::string getName() const;
        // int getTokenType() const;
        //int getToken() const;
    };
}
#endif //COMPILER_TOKEN_H


