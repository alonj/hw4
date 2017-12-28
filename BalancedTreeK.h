//
// Created by alonj on 23-Dec-17.
//
#include "Node.h"
#include <cstddef>

#ifndef HW4_KTREE_H
#define HW4_KTREE_H
class BalancedTreeK{
private:
    Node* root;
public:
    BalancedTreeK(const Key* min, const Key* max);
    ~BalancedTreeK();
    void Init();
    void Insert(const Key* nkey, const Value* nval);
    void Delete(const Key* dkey);
    Value* Search(const Key* key) const;

    unsigned Rank(const Key* key) const;
    const Key* Select(unsigned index) const;
    const Value* GetMaxValue(const Key* key1, const Key* key2) const;
};

#endif //HW4_KTREE_H
