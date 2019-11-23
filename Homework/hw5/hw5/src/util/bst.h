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

using namespace std;

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

   BSTreeNode(const T& d, BSTreeNode<T>* p = 0,
                          BSTreeNode<T>* l = 0,
                          BSTreeNode<T>* r = 0):
      _data(d), _parent(p), _left(l), _right(r) {}

   T              _data;
   BSTreeNode<T>* _parent;
   BSTreeNode<T>* _left;
   BSTreeNode<T>* _right;
};


template <class T>
class BSTree
{
public:
   // TODO: decide the initial value for _isSorted
   BSTree() : _size(0), _isSorted(false)
   {
      _root = new BSTreeNode<T>(T());
      _begin = new BSTreeNode<T>(T());
      _end = new BSTreeNode<T>(T());
      _root->_parent = _root->_left = _root->_right = _root;
   }
   ~BSTree() { clear(); }

   class iterator
   {
      friend class BSTree;

   public:
      iterator(BSTreeNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { return *(this); }
      iterator operator ++ (int) { return *(this); }
      iterator& operator -- () { return *(this); }
      iterator operator -- (int) { return *(this); }

      iterator& operator = (const iterator& i) { return *(this); }

      bool operator != (const iterator& i) const { return false; }
      bool operator == (const iterator& i) const { return false; }
   
   private:
      BSTreeNode<T>* _node;
   };

   iterator begin() const { return _root; }
   iterator end() const { return _root; }
   bool empty() const { return (_size == 0); }
   size_t size() const { return _size; }

   void insert(const T& x)
   { 
      if ((_root->_left == _root) && (_root->_right == _root))
      {
         _root->_left = _root->_right = 0;
         _root->_data = x;
         _begin = _end = _root;
         return;
      }
      _insert(x, _root, _root); 
      _size++;
   }
   void pop_front() { }
   void pop_back() { }

   // return false if nothing to erase
   bool erase(iterator pos) { return false; }
   bool erase(const T& x) { return false; }

   iterator find(const T& x) { return end(); }

   void clear() { }  // delete all nodes except for the dummy node

   void sort() const { }

   void print() const { }

private:
   BSTreeNode<T> *_root;
   BSTreeNode<T> *_begin, *_end;
   size_t _size;
   mutable bool _isSorted;
   
   BSTreeNode<T>* _insert(const T& x, BSTreeNode<T>* n, BSTreeNode<T>* pre_n)
   {
      if (n == NULL) return new BSTreeNode<T>(x, pre_n);
      if (x == n->_data) return n;
      if (x < n->_data) 
      {
         n->_left = _insert(x, n->_left, n);
         if (n == _begin) _begin = n->_left;
      }
      else 
      {
         n->_right = _insert(x, n->_right, n);
         if (n == _end) _end = n->_right;
      }
      return n;
   }

   void _search(const T& x) { }
   iterator _min(BSTreeNode<T>* n) { }
   void _successor(BSTreeNode<T>* n) { }
   bool _delete(iterator pos) { return false; }
};

#endif // BST_H
