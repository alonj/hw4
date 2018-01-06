//
// Created by alonj on 23-Dec-17.
//
#include "Key.h"
#include "BalancedTreeK.h"
#include "Node.h"
#include "Value.h"

Node* search_key(Key* key){
    Node* y = _root;
    while(!y->isLeaf()){
        int i=0;
        while(*(y->get_child(i)->get_key()) < *key && y->get_child(i)->get_key() != nullptr){
            i++;
        }
        y = y->get_child(i);
    }
    y = y->get_parent();
    int n = 0;
    while(y->get_child(n)->get_key() != key && n <= 2*ParameterK_H::k - 1){
        n++;
    }
    if(n == 2*ParameterK_H::k - 1){
        return nullptr;
    }
    else{
        return y->get_child(n);
    }
}

BalancedTreeK::BalancedTreeK(const Key *min, const Key *max){
    Init();
}

void BalancedTreeK::Init() {
    Key* minKey=min->clone();
    Key* maxKey=max->clone();
    Key* rootKey=new Key();
    Value defaultVal;
    Node minNode(*defaultVal, *minKey);
    Node maxNode(*defaultVal, *maxKey);
    Node root(*defaultVal, *maxKey);
    minNode.def_parent(root, true);
    maxNode.def_parent(root, true);
    this->_root=*root;
}

Node* insert_and_split(Node* y_parent, Node* new_node){
    int k = ParameterK_H::K;
    if(y_parent->get_child_count() < 2*k-1){
        new_node->set_parent(y_parent);
        return nullptr;
    }
    Node new_internal(y_parent->get_value(),y_parent->get_key()); // define key and value
    int place=0;
    int split_point=0;
    while(*(y->get_child(place)->get_key()) < *nkey && y->get_child(place)->get_key() != nullptr){
        place++;
    }
    if(place>k-1){
        split_point = k;
    }
    else{
        split_point = k-1;
    }
    for(i=split_point; i < 2*k - 1; i++){
        y_parent->get_child(i)->set_parent(new_internal);
    }
    return *new_internal;

}

void BalancedTreeK::Insert(const Key* nkey, const Value* nval){
    Node new_node(nval, nkey);
    Node* y = _root;
    while(!y->isLeaf()){
        int i=0;
        while(*(y->get_child(i)->get_key()) < *nkey && y->get_child(i)->get_key() != nullptr){
            i++;
        }
        y = y->get_child(i);
    }
    Node* x = y->get_parent();
    insert_and_split(x, new_node);
    while(x != _root){
        x = x->get_parent();
        if(new_node != nullptr){
            new_node = insert_and_split(x, new_node);
        }
        else {
            x->update_key();
        }
    }
    if(new_node != nullptr){
        Node new_root(_root->get_value(),_root->get_key());
        x->set_parent(new_root);
        new_node.set_parent(new_root);
        _root = new_root;
    }
}

Value* BalancedTreeK::Search(const Key *key) const {
    Node* y = search_key(key);
    if(y == nullptr){
        return nullptr;
    }
    else{
        return y->get_value();
    }
}

unsigned BalancedTreeK::Rank(const Key *key) const {
    Node* x = search_key(key);
    if (x != nullptr) {
        unsigned rank = 1;
        Node *y = x->get_parent();
        while (y != nullptr) {
            int i = 0;
            while (x->get_parent()->get_child(i) != x) {
                rank += x->get_parent()->get_child(i)->get_total_child();
                i++;
            }
            x = y;
            y = y->get_parent();
        }
        return rank;
    }
    else{
        return 0;
    }
}

const Key* BalancedTreeK::Select(unsigned index) const {
    Node* x = _root;
    if(x->get_total_child() < index){
        return nullptr;
    }
    else if(x->isLeaf()){
        return x->get_key();
    }
    int n = 0;
    while(!x->isLeaf()) { // TODO: count the statistic order
        while (index < x->get_child(n) + x->get_child(n + 1) && n <= x->get_child_count() - 1) {
            // index -=
            n++;
        }
        x = x->get_child(n);
    }
}