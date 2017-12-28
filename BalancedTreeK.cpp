//
// Created by alonj on 23-Dec-17.
//
#include "Key.h"
#include "BalancedTreeK.h"
#include "Node.h"
#include "Value.h"

Search_left(const Key* nkey){

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
    this->root=*root;
}

void BalancedTreeK::Insert(const Key* nkey, const Value* nval){
    Node new_node(nval, nkey);
    /*
     * Search_left // (binary search) find the left sibling of the new node
     * potential_parent = left_sibling.parent.children_counter() // is equal to 2k-1
     * if potential_parent == 2k1 then
     *      parent = potential_parent.right_sibling
     * else
     *      parent = potential_parent
     * parent.add_child(newnode.key){
     *      children_counter++;
     *      if newnode.key > parent.key then:
     *          parent.key = newnode.key
     *      if children_counter > 2k-1 then:
     *          current.parent.right_child = current.parent.right_sibling
     *          current.parent.add_child(current.key)-
     *
     *
     * }
     */
}