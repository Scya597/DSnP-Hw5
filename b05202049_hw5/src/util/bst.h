/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
  friend class BSTree<T>;
  friend class BSTree<T>::iterator;

  BSTreeNode(const T& d, BSTreeNode<T>* p = NULL, BSTreeNode<T>* q = NULL, BSTreeNode<T>* n = NULL):
      _data(d), _left(p), _right(q), _parent(n) {}

  T              _data;
  BSTreeNode<T>*  _left;
  BSTreeNode<T>*  _right;
  BSTreeNode<T>*  _parent;
};


template <class T>
class BSTree
{
public:
   // TODO: design your own class!!
   BSTree() { _root = 0; _dummy = 0; }
   ~BSTree() { clear(); delete _dummy; }

   class iterator {
     friend class BSTree;

     public:
       iterator(BSTreeNode<T>* n= 0): _node(n) {}
       iterator(const iterator& i) : _node(i._node) {}
       ~iterator() {}

       const T& operator * () const { return _node->_data; }
       T& operator * () { return _node->_data; }

       iterator& operator ++ () {
         if (_node->_right == NULL) {
           _node = findNextMaxOnTop(_node);
         } else if (_node->_right->_left != _node) {
           _node = min(_node->_right);
         } else {
           _node = _node->_right;
         }
         return *(this);
       }

       iterator operator ++ (int) {
         iterator next = *this;
         if (_node->_right == NULL) {
           _node = findNextMaxOnTop(_node);
         } else if (_node->_right->_left != _node) {
           _node = min(_node->_right);
         } else {
           _node = _node->_right;
         }
         return next;
       }

       iterator& operator -- () {
         if (_node->_left == NULL) {
           _node = findNextMinOnTop(_node);
         } else {
           _node = max(_node->_left);
         }
         return *(this);
       }

       iterator operator -- (int) {
         iterator next = *this;
         if (_node->_left == NULL) {
           _node = findNextMinOnTop(_node);
         } else {
           _node = max(_node->_left);
         }
         return next;
       }

       iterator& operator = (const iterator& i) { _node = i._node; return *(this); }

       bool operator != (const iterator& i) const { return (_node != i._node); }
       bool operator == (const iterator& i) const { return (_node == i._node); }

     private:
       BSTreeNode<T>* min(BSTreeNode<T>* Node) {
         if (Node->_left == NULL) {
           return Node;
         } else {
           return min(Node->_left);
         }
       }

       BSTreeNode<T>* max(BSTreeNode<T>* Node) {
         if (Node->_right == NULL || Node->_right->_left == Node) {
           return Node;
         } else {
           return max(Node->_right);
         }
       }

       BSTreeNode<T>* findNextMaxOnTop(BSTreeNode<T>* Node) {
         if (Node->_parent->_left == Node) {
           return Node->_parent;
         } else {
           if (Node->_parent->_parent == NULL) {
             return max(Node);
           }
           return findNextMaxOnTop(Node->_parent);
         }
       }

       BSTreeNode<T>* findNextMinOnTop(BSTreeNode<T>* Node) {
         if (Node->_parent->_right == Node) {
           return Node->_parent;
         } else {
           if (Node->_parent->_parent == NULL) {
             return min(Node);
           }
           return findNextMinOnTop(Node->_parent);
         }
       }

       BSTreeNode<T>* _node;
   };

   iterator begin() const {
     if (empty()) {
       return end();
     }
     return iterator(min(_root));
   }

   iterator end() const {
     return iterator(_dummy);
   }

   bool empty() const {
     if (_root == 0) {
       return true;
     }
     return false;
   }

   size_t size() const {
     if (empty()) { return 0; }
     size_t count = 0;
     for (iterator li = begin(); li != end(); li++) {
       count++;
     }
     return count;
   }

   void insert(const T& x) {
     BSTreeNode<T>* newNode = new BSTreeNode<T>(x);
     if (_root == 0) {
       _root = newNode;
       _dummy = new BSTreeNode<T>(x);
       _root->_right = _dummy;
       _dummy->_left = _root;
     } else {
       InsertAtNode(newNode, _root);
     }
   }

   void pop_front() {
     if (empty()) { return; }
     erase(begin());
   }

   void pop_back() {
     if (empty()) { return; }
     erase(--end());
   }

   bool erase(iterator pos) {
     if (empty()) { return false; }

     if (pos._node->_right == _dummy) {
       if (pos._node->_left == NULL && pos._node->_parent == NULL) {
         _root = 0;
       } else if (pos._node->_left != NULL && pos._node->_parent == NULL) {
         pos._node->_left->_parent = NULL;
         _root = pos._node->_left;
       } else if (pos._node->_left == NULL && pos._node->_parent != NULL) {
         pos._node->_parent->_right = _dummy;
       } else {
         pos._node->_left->_parent = pos._node->_parent;
         pos._node->_parent->_right = pos._node->_left;
       }
     } else if (pos._node->_left == NULL && pos._node->_right == NULL) {
       if (pos._node == _root) {
         _root = 0;
       } else if (pos._node->_parent->_left == pos._node) {
         pos._node->_parent->_left = NULL;
       } else {
         pos._node->_parent->_right = NULL;
       }
     } else if (pos._node->_left == NULL && pos._node->_right != NULL) {
       if (pos._node == _root) {
         _root = pos._node->_right;
         pos._node->_right->_parent = NULL;
       } else if (pos._node->_parent->_left == pos._node) {
         pos._node->_parent->_left = pos._node->_right;
         pos._node->_right->_parent = pos._node->_parent;
       } else {
         pos._node->_parent->_right = pos._node->_right;
         pos._node->_right->_parent = pos._node->_parent;
       }
     } else if (pos._node->_left != NULL && pos._node->_right == NULL) {
       if (pos._node == _root) {
         _root = pos._node->_left;
         pos._node->_left->_parent = NULL;
       } else if (pos._node->_parent->_left == pos._node) {
         pos._node->_parent->_left = pos._node->_left;
         pos._node->_left->_parent = pos._node->_parent;
       } else {
         pos._node->_parent->_right = pos._node->_left;
         pos._node->_left->_parent = pos._node->_parent;
       }
     } else {
       BSTreeNode<T>* replaceNode = successor(pos._node);
       if (replaceNode == pos._node->_right) {
         pos._node->_left->_parent = replaceNode;
         replaceNode->_left = pos._node->_left;
       } else if (replaceNode->_right != NULL) {
         replaceNode->_right->_parent = replaceNode->_parent;
         replaceNode->_parent->_left = replaceNode->_right;
         pos._node->_left->_parent = replaceNode;
         pos._node->_right->_parent = replaceNode;
         replaceNode->_left = pos._node->_left;
         replaceNode->_right = pos._node->_right;
       } else {
         pos._node->_left->_parent = replaceNode;
         pos._node->_right->_parent = replaceNode;
         replaceNode->_left = pos._node->_left;
         replaceNode->_right = pos._node->_right;
         replaceNode->_parent->_left = NULL;
       }

       if (pos._node == _root) {
         _root = replaceNode;
         replaceNode->_parent = NULL;
       } else {
         replaceNode->_parent = pos._node->_parent;
         if (pos._node->_parent->_left == pos._node) {
           pos._node->_parent->_left = replaceNode;
         } else {
           pos._node->_parent->_right = replaceNode;
         }
       }
     }

     if (_root != 0) {
       BSTreeNode<T>* maxNode = max(_root);
       maxNode->_right = _dummy;
       _dummy->_left = maxNode;
     } else {
       // delete _dummy;
       _dummy = 0;
     }

     delete pos._node;
     return true;
   }

   bool erase(const T& x) {
     if (empty()) { return false; }
     for (iterator li = begin(); li != end(); li++) {
       if (*li == x) {
         erase(li);
         return true;
       }
     }
     return false;
   }

   void clear() {
     if (empty()) { return; }
     while(_root != 0) {
       pop_front();
     }
   }

   void sort() const {}
   void print() const {}

private:
  BSTreeNode<T>* min(BSTreeNode<T>* Node) const {
    if (Node->_left == NULL) {
      return Node;
    } else {
      return min(Node->_left);
    }
  }

  BSTreeNode<T>* max(BSTreeNode<T>* Node) const {
    if (Node->_right == NULL || Node->_right == _dummy) {
      return Node;
    } else {
      return max(Node->_right);
    }
  }

  BSTreeNode<T>* successor(BSTreeNode<T>* Node) {
    return min(Node->_right);
  }

  void InsertAtNode(BSTreeNode<T>* newNode, BSTreeNode<T>* Node) {
    if (newNode->_data <= Node->_data) {
      if (Node->_left != NULL) {
        InsertAtNode(newNode, Node->_left);
      } else {
        Node->_left = newNode;
        newNode->_parent = Node;
      }
    } else {
      if (Node->_right == _dummy) {
        Node->_right = newNode;
        newNode->_parent = Node;
        newNode->_right = _dummy;
        _dummy->_left = newNode;
      } else if (Node->_right != NULL) {
        InsertAtNode(newNode, Node->_right);
      } else {
        Node->_right = newNode;
        newNode->_parent = Node;
      }
    };
  }

  BSTreeNode<T>*  _root;
  BSTreeNode<T>*  _dummy;
};

#endif // BST_H
