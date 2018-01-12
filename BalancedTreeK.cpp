//
// Created by alonj on 23-Dec-17.
//
#include "Key.h"
#include "BalancedTreeK.h"

/*
 * Get a pointer to Node of known Key
 * */
Node* BalancedTreeK::search_key(const Key* key) const{
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
    while(y->get_child(n)->get_key() != key && n <= 2*K - 1){
        n++;
    }
    if(n == 2*K - 1){
        return nullptr;
    }
    else{
        return y->get_child(n);
    }
}

/*
 * Initialize Balanced Tree structure with root and two children (min, max)
 * */
/*void Init(const Key *min, const Key *max) {
    Key* minKey=min->clone();
    Key* maxKey=max->clone();
    Key* rootKey=new Key();
    Value* defaultVal = new Value();
    Node minNode(nullptr, minKey);
    Node maxNode(nullptr, maxKey);
    Node* root = new Node;
    minNode.set_parent(root, true);
    maxNode.set_parent(root, true);
    this->_root=root;
}*/

/*
 * Constructor
 * */
BalancedTreeK::BalancedTreeK(const Key *min, const Key *max){
    Key* minKey=min->clone();
    Key* maxKey=max->clone();
    Node minNode(nullptr, minKey);
    Node maxNode(nullptr, maxKey);
    auto* root = new Node;
    minNode.set_parent(root, true);
    maxNode.set_parent(root, true);
    this->_root=root;
}


/*
 * Split nodes for Insert method
 * */
Node* insert_and_split(Node* y_parent, Node new_node){
    if(y_parent->get_child_count() < 2*K-1){
        new_node.set_parent(y_parent);
        return nullptr;
    }
    auto new_internal = new Node(y_parent->get_value(),y_parent->get_key()); // define key and value //todo WHAT's AUTO
    int place=0;
    int split_point=0;
    while(*(y_parent->get_child(place)->get_key()) < *new_node.get_key() && y_parent->get_child(place)->get_key() != nullptr){
        place++;
    }
    if(place>K-1){
        split_point = K;
    }
    else{
        split_point = K-1;
    }
    for(int i=split_point; i < 2*K - 1; i++){
        y_parent->get_child(i)->set_parent(new_internal);
    }
    return new_internal;

}


/*
 * Insert new node (key, val) into the tree
 * */
void BalancedTreeK::Insert(const Key* nkey, const Value* nval){
    Value* val1 = nval->clone();
    Key* key1 = nkey->clone();
    auto* new_node = new Node(val1, key1);
    Node* y = _root;
    while(!y->isLeaf()){
        int i=0;
        while(*(y->get_child(i)->get_key()) < *nkey && y->get_child(i)->get_key() != nullptr){
            i++;
        }
        y = y->get_child(i);
    }
    Node* x = y->get_parent();
    insert_and_split(x, *new_node);
    while(x != _root){
        x = x->get_parent();
        if(new_node != nullptr){ //TODO recheck the algorithm
            new_node = insert_and_split(x, *new_node);
        }
        else {
            x->update_key();
            x->update_val();
        }
    }
    if(new_node != nullptr){
        auto* new_root = new Node(_root->get_value(),_root->get_key());
        x->set_parent(new_root);
        new_node->set_parent(new_root);
        _root = new_root;
    }
}


/*
 * Search value by known Key
 * */
Value* BalancedTreeK::Search(const Key *key) const {
    return search_key(key)->get_value();
}


/*
 * Get ordered rank of known Key
 * */
unsigned BalancedTreeK::Rank(const Key *key) const {
    Node* x = search_key(key);
    if (x != nullptr) {
        unsigned rank = 1;
        Node *y = x->get_parent();
        while (y != nullptr) {
            int i =0;
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


/*
 * Get key of known ordered rank
 * */
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
        return x->get_key();
    }
}

void BalancedTreeK::Delete(const Key *dkey) {

}

/*
const Value* BalancedTreeK::GetMaxValue(const Key *key1, const Key *key2) const {
    int left_boundary;
    int right_boundary;
    unsigned index = 0;
    const Key* zero_key = this->Select(0);
    if(*(this->_root->get_key())<*key1 || *key2 < *key1)
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
            const Key *temp_key = this->Select(i);
            Value *temp_val = this->Search(temp_key);
            if (*max < *temp_val) {
                max = temp_val;
            }
        }
        return max;
    }
}*/

const Value* BalancedTreeK::GetMaxValue(const Key *key1, const Key *key2) const{
    if(*(this->_root->get_key())<*key1 || *key2 < *key1) {
        return nullptr;
    }
    Key* zero_key = this->Select(0)->clone();
    unsigned left_boundary = this->Rank(key1);
    unsigned right_boundary = this->Rank(key2);
    if (left_boundary == 0 && (*zero_key < *key1 || *key1 < *zero_key)) {
        Node *y = _root;
        while (!y->isLeaf()) {
            int i = 0;
            while (*(y->get_child(i)->get_key()) < *(key1) && y->get_child(i)->get_key() != nullptr) { // todo fix
                i++;
            }
            y = y->get_child(i);
        }
        left_boundary = this->Rank(y->get_key());
    }
    if (right_boundary == 0 && (*zero_key < *key2 || *key2 < *zero_key)) {
        Node *y = _root;
        while (!y->isLeaf()) {
            int i = 0;
            while (*(y->get_child(i)->get_key()) < *(key2) && y->get_child(i)->get_key() != nullptr) {
                i++;
            }
            y = y->get_child(i);
        }
        right_boundary = this->Rank(y->get_key()) - 1;
    }
    Value* max = nullptr;
    Node* x = search_key(this->Select(left_boundary));
    Node* x_parent = x->get_parent();
    while(*(x_parent->get_key()) < *this->Select(right_boundary)) { // define the while
        for(int i = 0; i < x_parent->get_child_count(); i++){
            if(*max < *(x_parent->get_child(i)->get_value()) && *x->get_key() < *(x_parent->get_child(i)->get_key())){
                max = x_parent->get_child(i)->get_value();
            }
        }
        x_parent = x->get_parent();
    }
    for(int i = left_boundary; i < x_parent->get_child_count(); i++){
        if(*max < *(x_parent->get_child(i)->get_value())){
            max = x_parent->get_child(i)->get_value();
        }
    }

    while(!x->isLeaf()){ // todo properly search key down the tree
        for(int i = 0; i < x_parent->get_child_count(); i++){
            if(*max < *(x_parent->get_child(i)->get_value()) && *(x_parent->get_child(i)->get_key()) < *x->get_key()){
                max = x_parent->get_child(i)->get_value();
            }
        }
    }
}