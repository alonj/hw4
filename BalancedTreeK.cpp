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
    if (*(y->get_key()) < *key) {            //********
        return nullptr;                      //***********
    }
    while(!y->isLeaf){
        int i=0;
        while (i < y->direct_children && *(y->get_child(i)->get_key()) < *key) {
            i++;
        }
        y = y->get_child(i);
    }
    y = y->get_parent();
    int n = 0;
    while(!(y->get_child(n)->compare_key(key)) && n < y->direct_children-1){
        n++;
    }
    if(n == 2*K - 1 or !(y->get_child(n)->compare_key(key))){
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
    Key* minKey = min->clone();
    Key* maxKey = max->clone();
    Node* minNode = new Node(nullptr, minKey);
    Node* maxNode = new Node(nullptr, maxKey);
    Node* root = new Node;
    minNode->set_parent(root, true);
    maxNode->set_parent(root, true);
    _root=root;
}

/*
 * Split nodes for Insert method
 * */
Node* insert_and_split(Node* x, Node* z){ // ex. : x=E, z=Z
    if(x->direct_children < 2*K - 1){

        z->set_parent(x);
        return nullptr;
    }
    else {
        Node* y = new Node();
        int split_point = K-1;

        /*
         * If new node is larger than mid-point node, split the array after the middle point (k-1 nodes remain in new parent)
         * */
        if (*(x->get_child(K-1)->get_key()) < *z->get_key()) {
            split_point++;
        }
        for (int i = split_point; i < 2 * K - 1; i++) {
            Node* child = x->get_child(split_point);
            child->set_parent(y);
        }
        if (*(x->get_key()) < *(z->get_key())) {
            z->set_parent(y);
        }
        else{
            z->set_parent(x);
        }
        y->update_attributes();
        x->update_attributes();
        return y;
    }
}

/*
 * Insert new node (key, val) into the tree
 * */
void BalancedTreeK::Insert(const Key* nkey, const Value* nval){
    Key* key;
    Value* val;
    if(nkey != nullptr && nval != nullptr){
        key = nkey->clone();
        val = nval->clone();
    }
    else {
        return;
    }
    Node* z = new Node(val, key);
    Node* y = _root;
    while(!y->get_child(0)->isLeaf){
        int i = 0;
        while(!(*nkey < *(y->get_child(i)->get_key()))){
            i++;
        }
        y = y->get_child(i);
    }
    // Node* x = y->get_parent();
    Node* x = y;
    z = insert_and_split(x, z);
    while(x != _root){
        x = x->get_parent();
        if(z != nullptr){
            z = insert_and_split(x, z);
        }
        else {
            x->update_attributes();
        }
    }
    if(z != nullptr){
        Node* new_root = new Node;
        z->update_attributes();
        z->set_parent(new_root);
        x->update_attributes();
        x->set_parent(new_root);
        _root = new_root;
    }

}


/*
 * Search value by known Key
 * */
Value* BalancedTreeK::Search(const Key *key) const {
    if(*(_root->get_key()) < *key ){
        return nullptr;
    }
    else{
        Node* result = search_key(key);
        if(result != nullptr){
            return result->get_value();
        }
        else{
            return nullptr;
        }
    }
}

/*
 * Get ordered rank of known Key
 * */
unsigned BalancedTreeK::Rank(const Key *key) const {
    if(*(_root->get_key())<*key) {
        return 0;
    }
    Node* x = search_key(key);
    if (x != nullptr) {
        unsigned rank = 1;
        Node *y = x->get_parent();
        while (y != nullptr) {
            int i = 0;
            while (y->get_child(i) != x) {
                rank += y->get_child(i)->total_children;
                i++;
            }
            x = y;
            y = y->get_parent();
        }
        return rank-1;
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
    index++;
    if(x->total_children-1 < index){
        return nullptr;
    }
    else if(x->isLeaf){
        return x->get_key();
    }
    else {
        while (!x->isLeaf) {
            int count_sum_siblings = 0;
            int i = 0;
            while (index > count_sum_siblings + x->get_child(i)->total_children) {
                count_sum_siblings += x->get_child(i)->total_children;
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
    if(y->direct_children + x->direct_children > 2*K-1){
        int i = 0;
        while(y->direct_children<K){
            x->get_child(i)->set_parent(y);
            i++;
        }
    }
    else{
        while(y->direct_children > 0){
            y->get_child(0)->set_parent(x);
        }
    }
    if(x->direct_children == 0){
        z->remove_child(x);
        delete x;
    }
    if(y->direct_children == 0){
        z->remove_child(y);
        delete y;
    }
    return z;
}

void BalancedTreeK::Delete(const Key *dkey) {
    Node* dNode = search_key(dkey);
    if (dNode == nullptr)                       //*************
    {
        return;                                //*************
    }
    Node* y = dNode->get_parent();
    y->remove_child(dNode);
    while(y != nullptr){
        if(y->direct_children<K){
            if(y != this->_root){
                y = borrow_and_merge(y);
            }
            else if(_root->direct_children == 1){
                this->_root = y->get_child(0);
                _root->set_parent(nullptr);
                delete y;
                return;
            }
            else{
                y->update_attributes();
                y = y->get_parent();
            }
        }
        else{
            y->update_attributes();
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
        while (!y->isLeaf) {
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
        while (!y->isLeaf) {
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
        for(int i = 0; i < x_parent->direct_children; i++){
            if(*max < *(x_parent->get_child(i)->get_value()) && *x->get_key() < *(x_parent->get_child(i)->get_key())){
                max = x_parent->get_child(i)->get_value();
            }
        }
        x_parent = x->get_parent();
    }
    for(int i = left_boundary; i < x_parent->direct_children; i++){
        if(*max < *(x_parent->get_child(i)->get_value())){
            max = x_parent->get_child(i)->get_value();
        }
    }

    while(!x->isLeaf){ // todo properly search key down the tree
        for(int i = 0; i < x_parent->direct_children; i++){
            if(*max < *(x_parent->get_child(i)->get_value()) && *(x_parent->get_child(i)->get_key()) < *x->get_key()){
                max = x_parent->get_child(i)->get_value();
            }
        }
    }
    return max;
}

BalancedTreeK::~BalancedTreeK() {
    // todo build destructor
    delete _root;
}
