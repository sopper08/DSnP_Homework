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
                          BSTreeNode<T>* r = 0,
                          BSTreeNode<T>* n = 0,
                          BSTreeNode<T>* v = 0):
      _data(d), _parent(p), _left(l), _right(r), _next(n), _prev(v) {}

   T              _data;
   BSTreeNode<T>* _parent;
   BSTreeNode<T>* _left;
   BSTreeNode<T>* _right;
   BSTreeNode<T>* _next;
   BSTreeNode<T>* _prev;
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
      iterator(BSTreeNode<T>* n= 0, BSTreeNode<T>* nn= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { _node = _node->_next; return *(this); }
      iterator operator ++ (int) { iterator tmp = *(this); _node = _node->_next; return tmp; }
      iterator& operator -- () { _node = _node->_prev; return *(this); }
      iterator operator -- (int) { iterator tmp = *(this); _node = _node->_prev; return tmp; }

      iterator& operator = (const iterator& i) { *(this)->_node = i->_node; return *(this); }

      bool operator != (const iterator& i) const { return _node != i._node; }
      bool operator == (const iterator& i) const { return _node == i._node; }
   
   private:
      BSTreeNode<T>* _node;
   };

   iterator begin() const { return iterator(_begin); }
   iterator end() const { return iterator(_end->_right); }
   bool empty() const { return (_size == 0); }
   size_t size() const { return _size; }

   void insert(const T& x)
   { 
      if ((_root->_left == _root) && (_root->_right == _root))
      {
         _root->_left = _root->_right = _root->_prev = _root->_next = 0;
         _root->_data = x;
         _begin = _end = _root;
         return;
      }
      BSTreeNode<T>* nN = _insert(x, _root, _root); 
      nN->_prev = _findPrevN(nN);
      nN->_next = _findNextN(nN);
      _begin = _min(_root);
      _end = _max(_root);

      
      _size++;


      for (iterator it = begin();
           it != end();
           it++)
      {
         T t = *it;
         cout << (*it) << endl;
      }
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
      if (n == NULL)
      {
         BSTreeNode<T>* result = new BSTreeNode<T>(x, pre_n);

         return result;
      }
      if (x == n->_data) return n;
      if (x < n->_data) 
      {
         n->_left = _insert(x, n->_left, n);
      }
      else 
      {
         n->_right = _insert(x, n->_right, n);
      }

      return n;
   }

   void _search(const T& x) { }

   BSTreeNode<T>* _min(BSTreeNode<T>* n) const
   { 
      if (n->_left == NULL) return n;
      else return _min(n->_left);
   }

   BSTreeNode<T>* _max(BSTreeNode<T>* n) const
   {
      if (n->_right == NULL) return n;
      else return _max(n->_right);
   }

   BSTreeNode<T>* _findNextN(BSTreeNode<T>* n) const
   {
      BSTreeNode<T>* nn;
      if (n->_right != NULL) nn = _min(n->_right);
      else nn = n->_parent;
      
      return nn;
   }

   BSTreeNode<T>* _findPrevN(BSTreeNode<T>* n) const
   {
      if (n->_left != NULL) { return _max(n->_left); }

      BSTreeNode<T>* prevN = n->_parent;
      while (prevN != NULL && n == prevN->_left)
      {
         n = prevN;
         prevN = n->_parent;
      }
      return prevN;
   }
   
   void _successor(BSTreeNode<T>* n) { }
   bool _delete(iterator pos) { return false; }
};

#endif // BST_H
