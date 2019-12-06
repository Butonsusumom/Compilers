#ifndef BLOCK_H
#define BLOCK_H
#include "node.h"
#include "text.h"

class Block : public Node {
public:
    static const unsigned long long MAX_CHILDREN_COUNT = 0xFFFFFF;

private:
    Node ** children;
    unsigned long long children_count;
    unsigned long long extended;

    void extend();

public:
    Block(const string &name, const Node * parent);
    ~Block();

    unsigned long long count() const;
    Node * add(const string &name);
    Node * add(const string &name, const string &text);
    Block * open(const string &name);
    Block * close();
    void portray(ostream &out, const unsigned int &depth);

    Node * operator [] (const unsigned long long &index) const;
};

#endif // BLOCK_H
