//
// Created by alonj on 23-Dec-17.
//

#include "ParameterK.h"
#include "Key.h"
#include "Value.h"
#include <cstddef>

#ifndef HW4_NODE_H
#define HW4_NODE_H
#define K Key_H::K;

class Node{
private:
    Bool isLeaf = true;
    Value* _value;
    int count_children=0;
    Key* _key;
    Node* _children[2K-1];
    Node* _parent;
public:
    Node(Value* nvalue, Key* nkey){
        _value=nvalue.clone();
        _key=nkey;};
    ~Node();
    void add_child(Node* child, int place);
    void remove_child(Node* child);
    void def_parent(Node* parent);
    Key* get_key(){return *_key;};
};

#endif //HW4_NODE_H
