#ifndef NODE_H
#define NODE_H
#include <iostream>
using namespace std;

class Node {
public:
    static const unsigned int MAX_ATTRIBUTES_COUNT = 256;
    enum Node_type {
        LEAF, BLOCK, ROOT
    };

protected:
    const Node * parent;
    const string name;
    Node_type type;
    pair<string, string> * attributes;
    unsigned int attributes_count;

public:
    Node(const string &name, const Node * parent);
    ~Node();

    string get_name() const;
    Node_type get_type() const;
    unsigned int attr_count() const;
    void add_attr(const string &key, const string &value);
    void set_last_attr(const string &value);
    void portray(ostream &out, const unsigned int &depth);

    pair<string, string> operator [] (const string &attr) const;
};

#endif // NODE_H
