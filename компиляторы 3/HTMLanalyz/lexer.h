#ifndef LEXER_H
#define LEXER_H
#include <iostream>
#include <fstream>
using namespace std;

class Lexer {
public:
    static const unsigned long long MAX_TOKENS_COUNT = 0xFFFFFF;
    static const unsigned long long MAX_BUFFER_SIZE = 0xFFFFFF;
    static const string block_tags[];
    static const string empty_tags[];
    static const unsigned int BLOCK_TAGS_COUNT = 97;
    static const unsigned int EMPTY_TAGS_COUNT = 16;

    enum Token_type {
        END = 1, TEXT = 2,
        OPENING_BLOCK_TAG_NAME = 4, CLOSING_BLOCK_TAG_NAME = 8, EMPTY_TAG_NAME = 16, COMMENT = 32, MACRO_TAG = 64,
        ATTRIBUTE_NAME = 128, UNQUOTED_ATTRIBUTE_VALUE = 256, SINGLE_QUOTED_ATTRIBUTE_VALUE = 512, DOUBLE_QUOTED_ATTRIBUTE_VALUE = 1024
    };
    struct Token {
        string lexeme;
        Token_type type;
        string type_str () {
            switch (type) {
            case END: return "END";
            case TEXT: return "TEXT";
            case OPENING_BLOCK_TAG_NAME: return "OPENING_BLOCK_TAG_NAME";
            case CLOSING_BLOCK_TAG_NAME: return "CLOSING_BLOCK_TAG_NAME";
            case EMPTY_TAG_NAME: return "EMPTY_TAG_NAME";
            case COMMENT: return "COMMENT";
            case MACRO_TAG: return "MACRO_TAG";
            case ATTRIBUTE_NAME: return "ATTRIBUTE_NAME";
            case UNQUOTED_ATTRIBUTE_VALUE: return "UNQUOTED_ATTRIBUTE_VALUE";
            case SINGLE_QUOTED_ATTRIBUTE_VALUE: return "SINGLE_QUOTED_ATTRIBUTE_VALUE";
            case DOUBLE_QUOTED_ATTRIBUTE_VALUE: return "DOUBLE_QUOTED_ATTRIBUTE_VALUE";
            }
            return "NONE";
        }
    };

private:
    Token * tokens;
    unsigned long long tokens_count;
    string buffer;
    unsigned long long curr_token_type;

    void process(const char &c);
    void add(const string &lexeme, const Token_type &type);
    void check_tag();

    void text();
    void block_tag_name();
    void empty_tag_name();
    void comment();
    void macro_tag();
    void attribute_name();
    void unquoted_attribute_value();
    void single_quoted_attribute_value();
    void double_quoted_attribute_value();


public:
    Lexer();
    Lexer(ifstream &file);
    ~Lexer();

    void disassemble(ifstream &file);
    unsigned long long count() const;
    Token operator [] (const unsigned long long &index) const;
};

#endif // LEXER_H
