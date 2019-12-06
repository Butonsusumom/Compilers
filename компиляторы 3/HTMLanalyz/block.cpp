
#include "block.h"

Block::Block (const string &name, const Node * parent) : Node(name, parent) , children_count(0) , extended(256) {
    this->parent = parent;
    type = BLOCK;
    children = new Node*[256];
}

Block::~Block () {
    delete [] children;
}




void Block::extend () {
    if (extended >= MAX_CHILDREN_COUNT)
        throw string("<" + name + "> has too many children!");

    extended *= 256;
    Node ** new_children = new Node*[extended];
    for (unsigned long long i(0); i < children_count; i++)
        new_children[i] = children[i];
    delete [] children;
    children = new_children;
}




unsigned long long Block::count () const {
    return children_count;
}

Node * Block::add (const string &name) {
    if (children_count == extended)
        extend();
    Node * node = new Node(name, this);
    children[children_count++] = node;
    return node;
}

Node * Block::add (const string &name, const string &text) {
    if (children_count == extended)
        extend();
    Text * node = new Text(name, text, this);
    children[children_count++] = node;
    return node;
}

Block * Block::open (const string &name) {
    if (children_count == extended)
        extend();
    Block * block = new Block(name, this);
    children[children_count++] = block;
    return block;
}

Block * Block::close () {
    return (Block*) parent;
}

void Block::portray (ostream &out, const unsigned int &depth) {
    for (unsigned int i(0); i < depth + 1; i++)
        out << "| ";
    out << endl;
    for (unsigned int i(0); i < depth; i++)
        out << "| ";
    out << "+--<" << name << ">" << endl;

    for (unsigned long long i(0); i < children_count; i++) {
        if (children[i]->get_type() == LEAF)
            children[i]->portray(out, depth+1);
        else
            static_cast<Block*>(children[i])->portray(out, depth+1);
    }
}




Node * Block::operator [] (const unsigned long long &index) const {
    if (index >= children_count)
        throw string("<" + name + "> child index is too large!");
    return children[index];
}
