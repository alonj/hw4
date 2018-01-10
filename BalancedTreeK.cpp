//
// Created by alonj on 23-Dec-17.
//
#include "Key.h"
#include "BalancedTreeK.h"
#include "Node.h"
#include "Value.h"
#include "ParameterK.h"

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
    while(*(y_parent->get_child(place)->get_key()) < *nkey && y_parent->get_child(place)->get_key() != nullptr){
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
    if(index > x->get_total_child()-1)
    {
        return nullptr;
    }
    else {
        while (!x->isLeaf()) { // TODO: count the statistic order
            int count_sum_siblings = 0;
            int i = 0;
            while (index > count_sum_siblings + x->get_child(i)->get_total_child()) {
                count_sum_siblings += x->get_child(i)->get_total_child();
                i++;
            }
            x = x->get_child(i);
            index -= count_sum_siblings;
        }
        return x;
    }
}

void BalancedTreeK::Delete(const Key *dkey) {
    
}

const Value* BalancedTreeK::GetMaxValue(const Key *key1, const Key *key2) const {
    int left_boundary;
    int right_boundary;
    unsigned index = 0;
    Key* zero_key = this->Select(0);
    if(*(this->_root->get_key())<*key1)
    {
        return nullptr;
    }
    else {
        if (*key1 < *key2) { // O(2*log(n))
            left_boundary = this->Rank(key1);
            right_boundary = this->Rank(key2);
            if (left_boundary == 0 && (*zero_key < *key1 || *key1 < *zero_key)) {
                left_boundary = -1;
            }
            if (right_boundary == 0 && (*zero_key < *key2 || *key2 < *zero_key)) {
                right_boundary = -1;
            }
        } else {
            left_boundary = this->Rank(key2);
            right_boundary = this->Rank(key1);
            if (left_boundary == 0 && (*zero_key < *key2 || *key2 < *zero_key)) {
                left_boundary = -1;
            }
            if (right_boundary == 0 && (*zero_key < *key1 || *key1 < *zero_key)) {
                right_boundary = -1;
            }
        }

        if (left_boundary == -1) {
            Node *y = _root;
            while (!y->isLeaf()) {
                int i = 0;
                while (*(y->get_child(i)->get_key()) < *left_boundary && y->get_child(i)->get_key() != nullptr) {
                    i++;
                }
                y = y->get_child(i);
            }
            left_boundary = this->Rank(y->get_key());
        }
        if (right_boundary == -1) {
            Node *y = _root;
            while (!y->isLeaf()) {
                int i = 0;
                while (*(y->get_child(i)->get_key()) < *left_boundary && y->get_child(i)->get_key() != nullptr) {
                    i++;
                }
                y = y->get_child(i);
            }
            right_boundary = this->Rank(y->get_key()) - 1;
        }
        Value *max = this->Search(key1);
        for (int i = left_boundary; i <= right_boundary; i++) {
            Key *temp_key = this->Select(i);
            Value *temp_val = this->Search(temp_key);
            if (*max < *temp_val) {
                max = temp_val;
            }
        }
        return max;
    }
}