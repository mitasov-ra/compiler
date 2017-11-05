//
// Created by ROMAN on 03.11.2017.
//

#ifndef COMPILER_TOKENTABLES_H
#define COMPILER_TOKENTABLES_H


#include <vector>
#include <string>

class TokenTables {
public:
    std::vector<std::string>    identifiers;
    std::vector<int>            integers;
    std::vector<std::string>    literals;

    int insertIdentifier(std::string &s);

    int insertInteger(std::string &s);

    int insertString(std::string &s);
};


#endif //COMPILER_TOKENTABLES_H
