//
// Created by alonj on 23-Dec-17.
//

#include "ParameterK.h"
#include "Key.h"
#include "Value.h"
#include <cstddef>

#ifndef HW4_NODE_H
#define HW4_NODE_H

class Node{
private:
    Bool _isLeaf = true;
    Value* _value;
    int count_children = 0;
    int total_children = 1;
    Key* _key;
    Node* _children[2*K-1];
    Node* _parent;
public:
    Node(Value* nvalue, Key* nkey){
        _value=nvalue.clone();
        _key=nkey;};
    ~Node();
    void add_child(Node* child, int place);
    void remove_child(Node* child);
    void set_parent(Node* parent);
    void set_key(Key* key){_key = key;};
    void update_key();
    const Key* get_key(){return _key;};
    const Key* get_value(){return _value;};
    Node* get_child(index){return _children[index];};
    const Node* get_parent(){return _parent;};
    const bool isLeaf(){return _isLeaf;};
    const int get_child_count(){return count_children;}; // returns number of children
    const int get_total_child(){return total_children;};
};

#endif //HW4_NODE_H
