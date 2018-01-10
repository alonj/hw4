//
// Created by alonj on 23-Dec-17.
//

#include "Node.h"
#include <cstddef>

using namespace std;

/*
 * find the place of the key in the children array to insert a new child
 */
int find_child_place(Node *parent, Node *child){
    int i = 0;
    Key* childKey = child->get_key();
    while(*(parent->get_child(i)->get_key())<*childKey && parent->get_child(i) != nullptr){
        i++;
    }
    return i;

}

/*
 * update key to match highest key of children
 * */
void Node::update_key(){
    int i = 0;
    while(this->get_child(i+1) != nullptr){
        i++;
    }
    this->set_key(this->get_child(i)->get_key());
}

void Node::update_val(){
    int i = 0;
    while(this->get_child(i) != nullptr){
        if (get_child(i)->get_value() > this->_value){
            this->_value = get_child(i)->get_value();
        }
    }
}

/*
 * inserts child to children array in correct place, moves keys "greater than" to the right
 */
void Node::add_child(Node *child, int place) {
    this->count_children++;
    for(i=count_children; i>place; i--){
        _children[i]=_children[i-1];
    }
    _children[place]=child;
    if(this->_isLeaf = true){
        this->_isLeaf = false;
        this->total_children = 0;
    }
    this->total_children = total_children + child->get_total_child();
}

/*
 * removes (known) child, moves all "greater than" to the left.
 * */
void Node::remove_child(Node *child) {
    int place = 0;
    for(i=0;i<count_children;i++){
        if(_children[i]==child){
            place=i;
        }
    }
    for(i=place;i<count_children;i++){
        _children[i]=_children[i+1];
    }
    count_children--;
    if(count_children==0){
        isLeaf = true;
    }
}

void Node::set_parent(Node *newParent, bool init = false) {
    int place=0;
    if(this->_parent!= nullptr)
            {
                this->_parent->remove_child(this);
            }
    this->_parent=newParent;
    if (init)
    {
        place=newParent->count_children;
    }
     else {
        place = find_child_place(*newParent, this);
    }
    newParent->add_child(this, place);
    this->update_key();
    this->update_val();
}
