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
        if(i != parent->direct_children -1){
            i++;
        }
        return i;
    }

}

/*
 * update key to match highest key of children
 * */
void Node::update_key(){
    if(!isLeaf) {
        this->set_key(this->get_child(direct_children - 1)->get_key()); //********** add ( befor this, ) after -1
        this->_minKey = this->get_child(0)->get_key();
    }
}

void Node::update_val(){
    if(this->direct_children == 1){
        _value = _children[0]->get_value();
    }
    else if(this->get_value() != nullptr) {
        for (int i = 0; i < this->direct_children; i++) {
            if (_value < _children[i]->get_value()){
                _value = _children[i]->get_value();
            }
        }
    }
}

void Node::update_attributes(){
    this->update_direct_children();
    this->update_total_children();
    this->isLeaf = this->direct_children == 0;
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
    total_children = direct_children;
    if (direct_children > 0 && !_children[0]->isLeaf) {
        total_children = 0;
        int i = 0;
        while (_children[i] != nullptr && i < 2 * K - 1) {
            total_children += _children[i]->total_children;
            i++;
        }
    }
}
/*
void Node::update_total_children() {
    if(direct_children > 0) {
        if (_children[0]->isLeaf) {
            total_children = direct_children;
        } else {
            total_children = 0;
            int i = 0;
            while (_children[i] != nullptr) {
				cout << "u_t_c line 74 i s: " << i << endl;
                total_children += _children[i]->total_children;
				cout << "to_chi now is: " << total_children << endl;
                i++;
				cout << "u_t_c now i is: " << i << endl;

            }
			cout << "u_t_c line 81" << endl;
        }
		cout << "u_t_c line 83" << endl;
    }
    else{
		cout << "u_t_c line 87" << endl;
        total_children = 0;
		cout << "u_t_c total ch is" << endl;
    }
}
*/

/*
 * inserts child to children array in correct place, moves keys "greater than" to the right
 */
void Node::add_child(Node* child, int place) {
    for(int i=direct_children; i>place; i--){
        _children[i]=_children[i-1];
    }
    _children[place]=child;
    this->update_attributes();
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
    for(int i=place;i<direct_children;i++){ // direct_children-1 ??
        if(i == direct_children-1){
            _children[i] = nullptr;
        }
        else{
            _children[i]=_children[i+1];
        }
    }
    _children[direct_children] = nullptr;  // new line add
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

void Node::set_parent(Node* newParent, bool init) {
    int place=0;
    if(_parent != nullptr) {  // ********************** who's _parent?
        _parent->remove_child(this);
    }
    if(newParent != nullptr) {
        _parent = newParent;          // ************* _parent of who??
        if (init) {
            newParent->add_child(this, newParent->direct_children);
        }
        else if (newParent->direct_children == 0) {
            newParent->add_child(this, 0);
        }
        else {
            if (*(_parent->get_key()) < *(this->_key)){
                place = _parent->direct_children;
                _parent->_children[place] = this;
                _parent->update_attributes();
            }
            else {
                place = find_child_place(newParent, this);
                newParent->add_child(this, place);
            }
        }
        this->update_attributes();
        _parent->update_attributes();
    }
}

bool Node::compare_key(const Key* other) const { // returns true if keys are same
    return !(*(this->_key) < *other || *(other) < *(this->_key));
}

Node::~Node(){
    /*delete isLeaf;
    delete direct_children;
    delete total_children;*/
    //cout << "Node destructor" << endl;
    delete _key;
    delete _value;
    for (int i = 0; i < 2 * K - 1; i++) {
        delete _children[i];
    }

    //delete _children array
}