//
// Created by alonj on 23-Dec-17.
//

#include <iostream>
#include "Node.h"

using namespace std;

/*
 * find the place of the key in the children array to insert a new child
 */
int find_child_place(Node *parent, Node *child){
    Key* childKey = child->get_key();
/*    while(*(parent->get_child(i)->get_key())<*childKey && parent->get_child(i) != nullptr){
        i++;
    }*/
    if(parent->direct_children == 0){
        return 0;
    }
    else {
        int i = parent->direct_children - 1;
        while (*childKey < *(parent->get_child(i)->get_key()) && i > 0) {
            i--;
        }
        return i;
    }

}

/*
 * update key to match highest key of children
 * */
void Node::update_key(){
    if(!isLeaf) {
        this->set_key(this->get_child(direct_children - 1)->get_key());
        this->_minKey = this->get_child(0)->get_key();
    }
}

void Node::update_val(){
    if(this->get_value() != nullptr) {
        for (int i = 0; i < this->direct_children; i++) {
            if (_value < _children[i]->get_value()){
                _value = _children[i]->get_value();
            }
        }
    }
}

void Node::update_attributes(){
    this->update_val();
    this->update_key();
}

void Node::update_direct_children(){
    direct_children = 0;
    while(_children[direct_children] != nullptr && direct_children < 2*K - 1){
        direct_children++;
    }
}

void Node::update_total_children() {
    if(_children[0]->isLeaf){
        total_children = direct_children;
    }
    else{
        total_children = 0;
        int i = 0;
        while(_children[i] != nullptr){
            total_children += _children[i]->total_children;
            i++;
        }
    }
}

/*
 * inserts child to children array in correct place, moves keys "greater than" to the right
 */
void Node::add_child(Node* child, int place) {
    for(int i=direct_children; i>place; i--){
        _children[i]=_children[i-1];
    }
    _children[place]=child;
    this->isLeaf = false;
    this->update_direct_children();
    this->update_total_children();
}

/*
 * removes (known) child, moves all "greater than" to the left.
 * */
void Node::remove_child(Node* child) {
    int place = 0;
    for(int i=0;i<direct_children;i++){
        if(_children[i]==child){
            place=i;
        }
    }
    for(int i=place;i<direct_children;i++){
        _children[i]=_children[i+1];
    }
    this->update_direct_children();
    this->update_total_children();
    for(int i = direct_children; i < 2*K-1; i++) {
        this->_children[i] = nullptr;
    }
    if(direct_children == 0){
        this->isLeaf = true;
        total_children = 0;
    }
}

void Node::nullify_child(int place){
    _children[place] = nullptr;
}


void Node::set_parent(Node* newParent, bool init) {
    int place=0;
    if(_parent != nullptr) {
        _parent->remove_child(this);
    }
    _parent=newParent;
    if (init) {
        place = newParent->direct_children;
    }
    else {
        place = find_child_place(newParent, this);
    }
    if(newParent->direct_children == 0){
        place = 0;
    }
    newParent->add_child(this, place);
    this->update_attributes();
    newParent->update_attributes();
    newParent->update_direct_children();
    newParent->update_total_children();
}

Node::~Node(){
    /*delete isLeaf;
    delete direct_children;
    delete total_children;*/
}