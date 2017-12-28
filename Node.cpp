//
// Created by alonj on 23-Dec-17.
//

#include "Node.h"
#include <cstddef>


/*
 * find the place of the key in the children array to insert a new child
 */
int find_child_place(Node *parent, Node *child){
    int i = 0;
    Key* childKey = child->get_key();
    while(*(parent._chilren[i])<*childKey){
        i++;
    }
    return i;
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
    this->isLeaf = false;
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

void Node::def_parent(Node *newParent, bool init=false) {
    int place=0;
    if(this->_parent!=NULL) //TODO CSTDDEF
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
    newParent->add_child(this, place); //TODO CHANGE TMPPLACE TO SEARCH RESULT
}