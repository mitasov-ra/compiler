//
// Created by ROMAN on 03.11.2017.
//

#include <algorithm>
#include "TokenTables.h"


int TokenTables::insertIdentifier(std::string &s) {
    auto it = std::find(identifiers.begin(), identifiers.end(), s);
    if (it != identifiers.end())
        return it - identifiers.begin();

    identifiers.push_back(s);
    return identifiers.size() - 1;
}

int TokenTables::insertInteger(std::string &s) {
    int num = atoi(s.c_str());
    auto it = std::find(integers.begin(), integers.end(), num);
    if (it != integers.end())
        return it - integers.begin();

    integers.push_back(num);
    return integers.size() - 1;
}

int TokenTables::insertString(std::string &s) {
    auto it = std::find(literals.begin(), literals.end(), s);
    if (it != literals.end())
        return it - literals.begin();

    literals.push_back(s);
    return literals.size() - 1;
}