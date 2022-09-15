//
// Created by Alaric on 2021/11/30.
//

#ifndef PROJECT_5_DLIST_IMPL_H
#define PROJECT_5_DLIST_IMPL_H
#include "dlist.h"
template <typename T>
Dlist<T>::Dlist() {
    first = nullptr;
    last = nullptr;
}
template <typename T>
bool Dlist<T>::isEmpty() const {
    return (first == nullptr && last == nullptr);
}

template <typename T>
void Dlist<T>::insertFront(T *op) {
    // MODIFIES: this
    // EFFECTS: if list is empty, add new node and set both `first` and `last` to the elem inserted;
    // else, insert normally, which set the prev to NULL yet the next to the original first
    node *nodeNew = new node;
    nodeNew->op = op; // set the value of the new node to op (the value, of course, is the address)
    if (isEmpty()){ /// previously empty
        nodeNew->prev = nodeNew->next = nullptr;
        first = last = nodeNew; // first and last both points to the same node
        return;
    }
    nodeNew->prev = nullptr; // set the NEW first's prev to the NULL
    nodeNew->next = first; // set the NEW first's next to the original FIRST node
    first->prev = nodeNew; // set the prev of the original first to the FIRST node
    first = nodeNew; // set the FIRST pointer to the NEW first node

}

template <typename T>
void Dlist<T>::insertBack(T *op) {
    // MODIFIES: this
    // EFFECTS: if list is empty, add new node and set both `first` and `last` to the elem inserted;
    // else, insert normally, which means set prev to the original last and next to NULL
    node *nodeNew = new node;
    nodeNew->op = op; // initialize the value of the node it points to
    if (isEmpty()){ /// previously empty
        nodeNew->prev = nodeNew->next = nullptr;
        first = last = nodeNew; // first and last both points to the same node
        return;
    }
    nodeNew->prev = last; // let the NEW last node points to the second last
    nodeNew->next = nullptr; // the NEW last node's next points to NULL
    last->next = nodeNew; // let the second to the last node points to the NEW last node
    last = nodeNew; // update the last ptr to the NEW last node

}

template <typename T>
T *Dlist<T>::removeFront() {
    // MODIFIES: this
    // EFFECTS: if list empty, throw exception; if only one elem is present, free this elem and set first
    // and last both to NULL; else, remove from the front, remembering to free and reset first and last
    /// to free the dynamic object: 1. delete first->op（the client's JOB!）  2. delete first (Programmer's JOB!)
    if (isEmpty()){ // empty list, throw error message
        emptyList err;
        throw err;
    }
    T* removeVal = first->op;
    // delete the first node, change the chain of pointer relation
    if ( first == last){ // only one element
        delete first;
        first = last = nullptr;
        return removeVal;
    }
    node *second = first->next; // extract the second node in the list
    second->prev = nullptr; // change the prev ptr of the second node of the original list
    delete first; // free the dynamic object stored in the first ptr
    first = second; // reset the first pointer to the NEW first elem
    return removeVal;
}

template <typename T>
T *Dlist<T>::removeBack() {
    // MODIFIES: this
    // EFFECTS: if list empty, throw exception; if only one elem is present, free this elem and set first
    // and last both to NULL; else, remove from the back, remembering to free and reset first and last
    if (isEmpty()){ // empty list, throw error message
        emptyList err;
        throw err;
    }
    T* removeVal = last->op; // value to be returned
    if (last == first) { // only one element in the list
        delete first;
        first = last = nullptr;
        return removeVal;
    }
    node *sec2Last = last->prev; // extract the second to last node, address, of course
    sec2Last->next = nullptr;
    delete last;
    last = sec2Last;
    return removeVal;
}

template <typename T>
void Dlist<T>::removeAll() {
    if (first == nullptr && last == nullptr) { // list empty
        return;
    }
    node *curPtr = first;
    while (curPtr != nullptr){ /// NOTE: the sequence of operations!
        curPtr = first->next;
        auto elt = removeFront(); // obtain dynamic object in this scope
        delete elt;
    }
}

template <typename T>
void Dlist<T>::copyAll(const Dlist<T> &l) {
    // MODIFIES: this
    // EFFECTS: make this the same as l
    // REQUIRES: current list is EMPTY
    node *cur = l.first;
    while (cur != nullptr){
        T *val = new T(*cur->op); // create dynamic object, achieving deep copy
        insertBack(val); // insert from the back to ensure the right sequence
        cur = cur->next;
    }
}

template <typename T>
Dlist<T>::~Dlist<T>() {
    removeAll();
}

template <typename T>
Dlist<T>::Dlist(const Dlist<T> &l) {
    first = last = nullptr;
    copyAll(l);
}

template <typename T>
Dlist<T> & Dlist<T>::operator=(const Dlist<T> &l) {
    if (isEmpty()){
        copyAll(l);
    }
    else{
        removeAll();
        copyAll(l);
    }
    return *this;
}

#endif //PROJECT_5_DLIST_IMPL_H
