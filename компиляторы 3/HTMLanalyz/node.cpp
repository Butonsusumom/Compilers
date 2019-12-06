
#include "node.h"

Node::Node (const string &name, const Node * parent) : name(name) , type(LEAF) , attributes_count(0) {
    this->parent = parent;
    attributes = new pair<string, string>[256];
}

Node::~Node () {
    delete [] attributes;
}




string Node::get_name () const {
    return name;
}

Node::Node_type Node::get_type () const {
    return type;
}

unsigned int Node::attr_count () const {
    return attributes_count;
}

void Node::add_attr (const string &key, const string &value) {
    if (attributes_count == MAX_ATTRIBUTES_COUNT)
        throw string("too many attributes!");

    attributes[attributes_count].first = key;
    attributes[attributes_count++].second = value;
}

void Node::set_last_attr (const string &value) {
    attributes[attributes_count-1].second = value;
}

void Node::portray (ostream &out, const unsigned int &depth) {
    for (unsigned int i(0); i < depth + 1; i++)
        out << "| ";
    out << endl;
    for (unsigned int i(0); i < depth; i++)
        out << "| ";
    out << "+--<" << name << ">" << endl;
}




pair<string, string> Node::operator [] (const string &attr) const {
    for (unsigned int i(0); i < attributes_count; i++)
        if (attributes[i].first == attr)
            return attributes[i];

    return pair<string, string>("NONE", "NONE");
}
