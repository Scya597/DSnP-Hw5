/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>
#include <iostream>
template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
      _isSorted = false;
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { _node = _node->_next; return *(this); }
      iterator operator ++ (int) { iterator next = *this; _node = _node->_next; return next; }
      iterator& operator -- () { _node = _node->_prev; return *(this); }
      iterator operator -- (int) { iterator next = *this; _node = _node->_prev; return next; }

      iterator& operator = (const iterator& i) { _node = i._node; return *(this); }

      bool operator != (const iterator& i) const { return (_node != i._node); }
      bool operator == (const iterator& i) const { return (_node == i._node); }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { return iterator(_head); }
   iterator end() const { return iterator(_head->_prev); }
   bool empty() const { return (_head == _head->_prev); }
   size_t size() const {
     size_t count = 0;
     for (iterator li = begin(); li != end(); li++) {
       count++;
     }
     return count;
   }

   void push_back(const T& x) {
     if (_head == _head->_prev) {
       DListNode<T>* newNode = new DListNode<T>(x, _head->_prev, _head);
       _head = newNode;
       _head->_prev->_next = _head;
       _head->_prev->_prev = _head;
     } else {
       DListNode<T>* newNode = new DListNode<T>(x, _head->_prev->_prev, _head->_prev);
       _head->_prev->_prev->_next = newNode;
       _head->_prev->_prev = newNode;
       _isSorted = false;
     }
   }

   void pop_front() {
     if (empty()) { return; }
     DListNode<T>* deleteNode = _head;
     (_head->_prev)->_next = _head->_next;
     (_head->_next)->_prev = _head->_prev;
     _head = _head->_next;
     delete deleteNode;
   }

   void pop_back() {
     if (empty()) { return; }
     DListNode<T>* deleteNode = _head->_prev->_prev;
     _head->_prev->_prev->_prev->_next = _head->_prev;
     _head->_prev->_prev = _head->_prev->_prev->_prev;
     delete deleteNode;
   }

   // return false if nothing to erase
   bool erase(iterator pos) {
     if (empty()) { return false; }
     if( pos._node == _head ) {
       pop_front();
       return true;
     } else {
       DListNode<T>* deleteNode = pos._node;
       pos._node->_prev->_next = pos._node->_next;
       pos._node->_next->_prev = pos._node->_prev;
       delete deleteNode;
       return true;
     }
   }

   bool erase(const T& x) {
     if (empty()) { return false; }
     for(iterator i = begin(); i != end(); i++) {
       if (*i == x) {
         erase(i);
         return true;
       }
     }
     return false;
   }

   void clear() {
     if (empty()) { return; }
     while(_head != _head->_prev) {
       pop_front();
     }
     std::cout << size() << std::endl;
   }  // delete all nodes except for the dummy node

   void sort() const {
     iterator a = begin();
     int length = size();
     for (size_t i = 0; i < length - 1; i++) {
       for (size_t j = 0; j < length - 1 - i; j++) {
         if (a._node->_data > a._node->_next->_data) {
           T temp = a._node->_data;
           a._node->_data = a._node->_next->_data;
           a._node->_next->_data = temp;
         }
         a++;
       }
       a = begin();
     }
     _isSorted = true;
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
