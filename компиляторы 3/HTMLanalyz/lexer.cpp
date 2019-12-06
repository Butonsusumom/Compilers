
#include "lexer.h"

const string Lexer::block_tags[] = {
    "a", "abbr", "address", "article", "aside", "audio", "b", "bdo", "blockquote", "body", "button", "canvas", "caption", "cite", "code", "colgroup", "data", "datalist", "dd", "del", "details", "dfn", "dialog", "div", "dl", "dt", "em", "fieldset", "figcaption", "figure", "footer", "form", "h1", "h2", "h3", "h4", "h5", "h6", "head", "header", "html", "i", "iframe", "ins", "kbd", "label", "legend", "li", "main", "map", "mark", "meter", "nav", "noscript", "object", "ol", "optgroup", "option", "output", "p", "picture", "pre", "progress", "q", "ruby", "rb", "rt", "rtc", "rp", "s", "samp", "script", "section", "select", "small", "span", "strong", "style", "sub", "summary", "sup", "table", "tbody", "td", "template", "textarea", "tfoot", "th", "thead", "time", "title", "tr", "track", "u", "ul", "var", "video"
};

const string Lexer::empty_tags[] = {
    "!DOCTYPE", "area", "base", "br", "col", "embed", "hr", "img", "input", "link", "menuitem", "meta", "param", "source", "track", "wbr"
};




Lexer::Lexer () : tokens_count(0) {
    tokens = new Token[MAX_TOKENS_COUNT];
    buffer = new char[MAX_BUFFER_SIZE];
}

Lexer::Lexer (ifstream &file) : Lexer() {
    disassemble(file);
}

Lexer::~Lexer () {
    delete [] tokens;
}




void Lexer::process (const char &c) {
    switch (curr_token_type) {
    case END: {
        throw string("unexpected ending!");
    break; }
    case TEXT: {
        if (c == '>')
            throw string("unexpected symbol: \">\"!");

        else if (c == '<') {
            if (!buffer.empty()) {
                add(buffer, TEXT);
                buffer.clear();
            }
            curr_token_type = OPENING_BLOCK_TAG_NAME | CLOSING_BLOCK_TAG_NAME | EMPTY_TAG_NAME | COMMENT | MACRO_TAG;

        } else
            buffer.push_back(c);
    break; }
    case OPENING_BLOCK_TAG_NAME: {
        throw string("error!");
    break; }
    case CLOSING_BLOCK_TAG_NAME: {
        if (c == '<')
            throw string("unexpected symbol: \"<\"!");
        else if (c == '/')
            throw string("unexpected symbol: \"<\"!");
        else if (c == '!')
            throw string("unexpected symbol: \"!\"!");
        else if (c == '?')
            throw string("unexpected symbol: \"?\"!");
        else if (c == ' ')
            throw string("unexpected symbol: \" \"!");
        else if (c == '\t')
            throw string("unexpected symbol: \"\\t\"!");
        else if (c == '\n')
            throw string("unexpected symbol: \"\\n\"!");
        else if (c == '>') {
            for (unsigned int i(0); i < BLOCK_TAGS_COUNT; i++)
                if (buffer == block_tags[i]) {
                    add(buffer, CLOSING_BLOCK_TAG_NAME);
                    buffer.clear();
                    curr_token_type = TEXT;
                    break;
                }
        } else
            buffer.push_back(c);
    break; }
    case EMPTY_TAG_NAME: {
        throw string("error!");
    break; }
    case COMMENT: {
        if (c == '>' && buffer.size() > 1 && buffer[buffer.size()-1] == '-' && buffer[buffer.size()-2] == '-') {
            add(buffer.substr(0, buffer.size()-2), COMMENT);
            buffer.clear();
            curr_token_type = TEXT;

        } else
            buffer.push_back(c);
    break; }
    case MACRO_TAG: {
        if (c == '>' && buffer[buffer.size()-1] == '?') {
            add(buffer.substr(0, buffer.size()-1), MACRO_TAG);
            buffer.clear();
            curr_token_type = TEXT;

        } else
            buffer.push_back(c);
    break; }
    case OPENING_BLOCK_TAG_NAME | CLOSING_BLOCK_TAG_NAME | EMPTY_TAG_NAME | COMMENT | MACRO_TAG: {
        if (c == '!') {
            if (buffer.empty()) {
                buffer.push_back('!');
                curr_token_type = EMPTY_TAG_NAME | COMMENT;
            } else
                throw string("unexpected symbol: \"!\"!");

        } else if (c == '?') {
            if (buffer.empty())
                curr_token_type = MACRO_TAG;
            else
                throw string("unexpected symbol: \"?\"!");
        } else if (c == '>') {
            if (buffer.empty())
                throw string("empty tag!");
            else {
                check_tag();
                curr_token_type = TEXT;
            }
        } else if (c == '/') {
            if (buffer.empty())
                curr_token_type = CLOSING_BLOCK_TAG_NAME;
            else
                check_tag();

        } else if (c == ' ' || c == '\t' || c == '\n') {
            if (!buffer.empty())
                check_tag();

        } else
            buffer.push_back(c);
    break; }
    case EMPTY_TAG_NAME | COMMENT: {
        buffer.push_back(c);
        if (buffer == "!--") {
            curr_token_type = COMMENT;
            buffer.clear();
            return;
        } else if (c == ' ' || c == '\t' || c == '\n') {
            string buf = buffer.substr(0, buffer.size()-1);
            for (unsigned int i(0); i < EMPTY_TAGS_COUNT; i++)
                if (buf == empty_tags[i]) {
                    add(buf, EMPTY_TAG_NAME);
                    buffer.clear();
                    curr_token_type = ATTRIBUTE_NAME;
                    break;
                }
        }
    break; }
    case ATTRIBUTE_NAME: {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (!buffer.empty()) {
                add(buffer, ATTRIBUTE_NAME);
                buffer.clear();
                curr_token_type = ATTRIBUTE_NAME | UNQUOTED_ATTRIBUTE_VALUE | SINGLE_QUOTED_ATTRIBUTE_VALUE | DOUBLE_QUOTED_ATTRIBUTE_VALUE;
            }

        } else if (c == '=') {
            add(buffer, ATTRIBUTE_NAME);
            buffer.clear();
            curr_token_type = UNQUOTED_ATTRIBUTE_VALUE | SINGLE_QUOTED_ATTRIBUTE_VALUE | DOUBLE_QUOTED_ATTRIBUTE_VALUE;

        } else if (c == '<')
            throw string("unexpected symbol: \"<\"!");

        else if (c == '>') {
            if (!buffer.empty()) {
                add(buffer, ATTRIBUTE_NAME);
                buffer.clear();
            }
            curr_token_type = TEXT;

        } else if (c == '/') {
            if (!buffer.empty()) {
                add(buffer, ATTRIBUTE_NAME);
                buffer.clear();
            }

        } else if (c == '!') {
            throw string("unexpected symbol: \"!\"!");

        } else if (c == '?') {
            throw string("unexpected symbol: \"?\"!");

        } else
            buffer.push_back(c);
    break; }
    case ATTRIBUTE_NAME | UNQUOTED_ATTRIBUTE_VALUE | SINGLE_QUOTED_ATTRIBUTE_VALUE | DOUBLE_QUOTED_ATTRIBUTE_VALUE: {
        if (c != ' ' && c != '\t' && c != '\n') {
            if (c == '=')
                curr_token_type = UNQUOTED_ATTRIBUTE_VALUE | SINGLE_QUOTED_ATTRIBUTE_VALUE | DOUBLE_QUOTED_ATTRIBUTE_VALUE;

            else if (c == '<')
                throw string("unexpected symbol: \"<\"!");

            else if (c == '>') {
                if (!buffer.empty()) {
                    add(buffer, ATTRIBUTE_NAME);
                    buffer.clear();
                }
                curr_token_type = TEXT;

            } else if (c == '/') {
                if (!buffer.empty()) {
                    add(buffer, ATTRIBUTE_NAME);
                    buffer.clear();
                }

            } else if (c == '!') {
                throw string("unexpected symbol: \"!\"!");

            } else if (c == '?') {
                throw string("unexpected symbol: \"?\"!");

            } else {
                curr_token_type = ATTRIBUTE_NAME;
                buffer.push_back(c);
            }
        }
    break; }
    case UNQUOTED_ATTRIBUTE_VALUE | SINGLE_QUOTED_ATTRIBUTE_VALUE | DOUBLE_QUOTED_ATTRIBUTE_VALUE: {
        if (c != ' ' && c != '\t' && c != '\n') {
            if (c == '\'') {
                if (buffer.empty())
                    curr_token_type = SINGLE_QUOTED_ATTRIBUTE_VALUE;
                else
                    throw string("unexpected symbol: \"'\"!");

            } else if (c == '"') {
                if (buffer.empty())
                    curr_token_type = DOUBLE_QUOTED_ATTRIBUTE_VALUE;
                else
                    throw string("unexpected symbol: \"\"\"!");

            } else if (c == '<') {
                throw string("unexpected symbol: \"<\"!");

            } else if (c == '>') {
                throw string("unexpected symbol: \">\"!");

            } else if (c == '/') {
                throw string("unexpected symbol: \"/\"!");

            } else if (c == '!') {
                throw string("unexpected symbol: \"!\"!");

            } else if (c == '?') {
                throw string("unexpected symbol: \"?\"!");

            } else if (buffer.empty()) {
                curr_token_type = UNQUOTED_ATTRIBUTE_VALUE;
                buffer.push_back(c);

            } else
                buffer.push_back(c);
        }
    break; }
    case UNQUOTED_ATTRIBUTE_VALUE: {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (!buffer.empty()) {
                add(buffer, UNQUOTED_ATTRIBUTE_VALUE);
                buffer.clear();

            } else if (c == '<') {
                throw string("unexpected symbol: \"<\"!");

            } else if (c == '>') {
                add(buffer, UNQUOTED_ATTRIBUTE_VALUE);
                buffer.clear();
                curr_token_type = TEXT;

            } else if (c == '/') {
                add(buffer, UNQUOTED_ATTRIBUTE_VALUE);
                buffer.clear();

            } else if (c == '!') {
                throw string("unexpected symbol: \"!\"!");

            } else if (c == '?') {
                throw string("unexpected symbol: \"?\"!");

            } else
                buffer.push_back(c);
        }
    break; }
    case SINGLE_QUOTED_ATTRIBUTE_VALUE: {
        if (c == '\'') {
            add(buffer, SINGLE_QUOTED_ATTRIBUTE_VALUE);
            buffer.clear();
            curr_token_type = ATTRIBUTE_NAME;

        } else
            buffer.push_back(c);
    break; }
    case DOUBLE_QUOTED_ATTRIBUTE_VALUE: {
        if (c == '"') {
            add(buffer, DOUBLE_QUOTED_ATTRIBUTE_VALUE);
            buffer.clear();
            curr_token_type = ATTRIBUTE_NAME;

        } else
            buffer.push_back(c);
    break; }
    }
}

void Lexer::add (const string &lexeme, const Token_type &type) {
    if (tokens_count == MAX_TOKENS_COUNT)
        throw string("tokens overflow!");
    tokens[tokens_count].lexeme = lexeme;
    tokens[tokens_count++].type = type;
    cerr << "[ \"" << lexeme << "\" : " << tokens[tokens_count-1].type_str() << endl;
}

void Lexer::check_tag () {
    unsigned int i(0);

    for (; i < BLOCK_TAGS_COUNT; i++)
        if (buffer == block_tags[i]) {
            add(buffer, OPENING_BLOCK_TAG_NAME);
            buffer.clear();
            curr_token_type = ATTRIBUTE_NAME;
            return;
        }

    for (i = 0; i < EMPTY_TAGS_COUNT; i++)
        if (buffer == empty_tags[i]) {
            add(buffer, EMPTY_TAG_NAME);
            buffer.clear();
            curr_token_type = ATTRIBUTE_NAME;
            return;
        }

    throw string("nonexistent tag: " + buffer);
}




void Lexer::disassemble (ifstream &file) {
    tokens_count = 0;
    curr_token_type = 0;

    unsigned long line(1), pos(1);
    try {
        char c;
        curr_token_type = TEXT;

        while ((c = file.get()) != EOF) {
            if (c == '\n') {
                pos = 1;
                line++;
            } else
                pos++;
            process(c);
        }

        if (buffer.size() != 0) {
            if (!(curr_token_type | TEXT))
                throw string("text was expected!");
            add(buffer, TEXT);
            buffer.clear();
        }

        add("", END);
    } catch (const string &error) {
        throw string("lexer: " + to_string(line) + "," + to_string(pos) + ": " + error);
    }
}

unsigned long long Lexer::count () const {
    return tokens_count;
}

Lexer::Token Lexer::operator [] (const unsigned long long &index) const {
    if (index >= tokens_count)
        throw string("token index is too large!");
    return tokens[index];
}
