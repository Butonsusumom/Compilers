
#include "text.h"

Text::Text (const string &name, const string &text, const Node * parent) : Node(name, parent) , text(text) {}




string Text::get_text () const {
    return text;
}
