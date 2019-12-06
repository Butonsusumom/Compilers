
#include "parser.h"

const string Parser::text_tags[] = {
    "h1", "h2", "h3", "h4", "h5", "h6", "b", "em", "i", "small", "strong", "sub", "sup", "ins", "del", "code", "kbd", "samp", "var", "pre", "abbr", "bdo", "blockquote", "q", "cite", "dfn", "p", "mark", "small"
};

Parser::Parser () {
    tree = new Root;
}

Parser::Parser (const Lexer &lexer) : Parser() {
    parse(lexer);
}

Parser::Parser (ifstream &file) : Parser() {
    parse(file);
}

Parser::~Parser () {
    delete tree;
}



void Parser::parse (const Lexer &lexer) {
    Block * open_block = (Block*) tree;
    Node * last_node = (Node*) tree;

    try {
        unsigned long long size = lexer.count();
        for (unsigned long long i(0); i < size-2; i++) {
            switch (lexer[i].type) {
            case Lexer::TEXT: {
                for (unsigned int j(0); j < TEXT_TAGS_COUNT; j++)
                    if (open_block->get_name() == text_tags[j])
                        last_node = open_block->add("TEXT", lexer[i].lexeme);
            break; }
            case Lexer::OPENING_BLOCK_TAG_NAME: {
                last_node = open_block = open_block->open(lexer[i].lexeme);
            break; }
            case Lexer::CLOSING_BLOCK_TAG_NAME: {
                if (lexer[i].lexeme != open_block->get_name())
                    throw string("unexpected closing tag: </" + lexer[i].lexeme + ">");
                open_block = open_block->close();
            break; }
            case Lexer::EMPTY_TAG_NAME: {
                last_node = open_block->add(lexer[i].lexeme);
            break; }
            case Lexer::COMMENT: {
                last_node = open_block->add("COMMENT", lexer[i].lexeme);
            break; }
            case Lexer::MACRO_TAG: {
                last_node = open_block->add("MACRO", lexer[i].lexeme);
            break; }
            case Lexer::ATTRIBUTE_NAME: {
                last_node->add_attr(lexer[i].lexeme, lexer[i].lexeme);
            break; }
            case Lexer::UNQUOTED_ATTRIBUTE_VALUE: {
                last_node->set_last_attr(lexer[i].lexeme);
            break; }
            case Lexer::SINGLE_QUOTED_ATTRIBUTE_VALUE: {
                last_node->set_last_attr(lexer[i].lexeme);
            break; }
            case Lexer::DOUBLE_QUOTED_ATTRIBUTE_VALUE: {
                last_node->set_last_attr(lexer[i].lexeme);
            break; }
            case Lexer::END: {
                if (open_block->get_type() != Node::ROOT)
                    throw string("unexpected ending!");
                open_block->close();
            }
            }
        }
    } catch (const string &error) {
        throw string("parser: " + error);
    }
}

void Parser::parse (ifstream &file) {
    parse(Lexer(file));
}

unsigned long long Parser::count () const {
    return tree->count();
}

void Parser::portray (ostream &out) const {
    unsigned int depth(0);
    tree->portray(out, depth);
}
