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
 * Constructor
 * */
BalancedTreeK::BalancedTreeK(const Key *min, const Key *max){
    auto* minKey=min->clone();
    auto* maxKey=max->clone();
    auto* minNode = new Node(nullptr, minKey);
    auto* maxNode = new Node(nullptr, maxKey);
    auto* root = new Node;
    minNode->set_parent(root, true);
    maxNode->set_parent(root, true);
    this->_root=root;
    cout<<"Tree created"<<endl;
}


/*
 * Split nodes for Insert method
 * */
Node* insert_and_split(Node* y_parent, Node new_node){
    if(y_parent->get_child_count() < 2*K-1){
        new_node.set_parent(y_parent);
        return nullptr;
    }
    auto* new_internal = new Node();
    int split_point = K;

    /*
     * If new node is larger than mid-point node, split the array after the middle point (k-1 nodes remain in new parent)
     * */
    if(*(y_parent->get_child(K)->get_key()) < *new_node.get_key()) {
        split_point++;
    }
    for(int i=split_point; i < 2*K - 2; i++){
        y_parent->get_child(i)->set_parent(new_internal);
    }
    return new_internal;

}


/*
 * Insert new node (key, val) into the tree
 * */
void BalancedTreeK::Insert(const Key* nkey, const Value* nval){
    Key* key = nkey->clone();
    Value* val = nval->clone();
    auto* z = new Node(val, key);
    Node* y = this->_root;
    while(!y->isLeaf()){
        int i=y->get_child_count()-1;
        while(*key < *(y->get_child(i)->get_key()) && i > 0){
            i--;
        }
        y = y->get_child(i);
    }
    Node* x = y->get_parent();
    z = insert_and_split(x, *z);
    while(x != this->_root){
        x = x->get_parent();
        if(z != nullptr){ //TODO recheck the algorithm
            z = insert_and_split(x, *z);
        }
        else {
            x->update_key();
            x->update_val();
        }
    }
    if(z != nullptr){
        auto* new_root = new Node;
        x->set_parent(new_root);
        z->set_parent(new_root);
        this->_root = new_root;
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
    if(index > x->get_total_child()-1)
    {
        return nullptr;
    }
    else {
        while (!x->isLeaf()) {
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

Node* borrow_and_merge(Node* y){
    Node* z = y->get_parent();
    int y_place = 0;
    Node* x = nullptr;
    while(z->get_child(y_place) != y){
        y_place++;
    }
    if(y_place == 0){
        x = z->get_child(y_place+1);
    }
    else{
        x = z->get_child(y_place-1);
    }
    if(y->get_child_count() + x->get_child_count() > 2*K-1){
        int i = 0;
        while(y->get_child_count()<K){
            x->get_child(i)->set_parent(y);
            i++;
        }
    }
    else{
        int i = 0;
        while(y->get_child_count() > 0){
            y->get_child(i)->set_parent(x);
            i++;
        }
    }
    if(x->get_child_count() == 0){
        z->remove_child(x);
        delete x;
    }
    if(y->get_child_count() == 0){
        z->remove_child(y);
        delete y;
    }

}

void BalancedTreeK::Delete(const Key *dkey) {
    Node* dNode = search_key(dkey);
    Node* y = dNode->get_parent();
    y->remove_child(dNode);
    while(y != nullptr){
        if(y->get_child_count()<K){
            if(y != this->_root){
                y = borrow_and_merge(y);
            }
            else{
                this->_root = y->get_child(0);
                y->set_parent(nullptr);
                delete y;
                return;
            }
        }
        else{
            y->update_key();
            y = y->get_parent();
        }
    }
}

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
            if(*key2 < *(y->get_minKey())){
                i--;
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

BalancedTreeK::~BalancedTreeK() {
    // todo build destructor
}