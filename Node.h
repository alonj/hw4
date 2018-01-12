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
    bool _isLeaf = true;
    Value* _value;
    int count_children = 0;
    int total_children = 1;
    Key* _key;
    Node* _children[2*K-1];
    Node* _parent;
public:
    explicit Node(Value* nvalue = nullptr, Key* nkey = nullptr){
        _value=nvalue->clone();
        _key=nkey;
        _parent = nullptr;
        for (auto &i : _children) {
                i = nullptr;
        }};
    ~Node();
    void add_child(Node* child, int place);
    void remove_child(Node* child);
    void set_parent(Node *newParent, bool init = false);
    void set_key(Key* key){_key = key;};
    void update_key();
    void update_val();
    Key* get_key(){return _key;};
    Value* get_value(){return _value;};
    Node* get_child(int index){return _children[index];};
    Node* get_parent(){return _parent;};
    const bool isLeaf(){return _isLeaf;};
    const int get_child_count(){return count_children;}; // returns number of children
    const int get_total_child(){return total_children;};
};

#endif //HW4_NODE_H
