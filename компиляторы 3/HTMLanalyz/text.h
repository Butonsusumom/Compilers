#ifndef TEXT_H
#define TEXT_H
#include "node.h"
using namespace std;

class Text : public Node {
private:
    const string text;

public:
    Text(const string &name, const string &text, const Node * parent);

    string get_text() const;
};

#endif // TEXT_H
