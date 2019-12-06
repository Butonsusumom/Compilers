#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include "lexer.h"
#include "root.h"
using namespace std;

class Parser {
public:
    static const string text_tags[];
    static const unsigned int TEXT_TAGS_COUNT = 29;
private:
    Root * tree;

public:
    Parser();
    Parser(const Lexer &lexer);
    Parser(ifstream &file);
    ~Parser();

    void parse(const Lexer &lexer);
    void parse(ifstream &file);
    unsigned long long count() const;
    void portray(ostream &out) const;
};

#endif // PARSER_H
