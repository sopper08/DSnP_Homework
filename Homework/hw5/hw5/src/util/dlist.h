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
   DList() : _isSorted(false)
   {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
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
      iterator operator ++ (int) { iterator tmp = *(this); _node = _node->_next; return tmp; }
      iterator& operator -- () { _node = _node->_prev; return *(this); }
      iterator operator -- (int) { iterator tmp = *(this); _node = _node->_prev; return tmp; }

      iterator& operator = (const iterator& i) { *(this)->_node = i->_node; return *(this); }

      bool operator != (const iterator& i) const { return _node != i._node; }
      bool operator == (const iterator& i) const { return _node == i._node; }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { return _head->_next; }
   iterator end() const { return _head; }
   bool empty() const 
   {
      if ((_head->_next == _head) && (_head->_prev == _head)) return true;
      else return false;
   }
   size_t size() const
   {
      if (empty()) return 0;
      size_t s = 0;
      iterator it = begin();
      while (it != end()) { s++; it++; }
      return s;
   }

   void push_back(const T& x) 
   { 
      DListNode<T> *t = new DListNode<T>(x, 0, _head);
      if (empty())
      {
         _head->_next = t;
         t->_prev = t->_next = _head;
      }
      else
      {
         DListNode<T>* _tail = _head->_prev;
         _tail->_next = t;
         t->_prev = _tail;
      }
      _head->_prev = t;
   }
   void pop_front() 
   {
      if (empty()) return;
      DListNode<T>* _newBegin = _head->_next->_next;
      delete _head->_next;
      _head->_next = _newBegin;
      _newBegin->_prev = _head;
   }
   void pop_back() 
   { 
      if (empty()) return;
      DListNode<T>* _newEnd = _head->_prev->_prev;
      delete _head->_prev;
      _head->_prev = _newEnd;
      _newEnd->_next = _head;
   }

   // return false if nothing to erase
   bool erase(iterator pos) 
   {  
      iterator it(pos);
      if (it == end()) return false;

      DListNode<T>* next_node = it._node->_next;
      DListNode<T>* prev_node = it._node->_prev;

      next_node->_prev = it._node->_prev;
      prev_node->_next = it._node->_next;

      delete &(*it);
      
      return true;
   }
   bool erase(const T& x) 
   {
      if (empty()) return false;
      iterator it = find(x);
      if (it == end()) return false;

      DListNode<T>* next_node = it._node->_next;
      DListNode<T>* prev_node = it._node->_prev;

      next_node->_prev = it._node->_prev;
      prev_node->_next = it._node->_next;

      delete &(*it);
      
      return true;
   }

   iterator find(const T& x) 
   {  
      for (iterator it = begin();
           it != end();
           it++)
      {
         if (*it == x) return it;
      }
      return end();
   }

   void clear()  // delete all nodes except for the dummy node
   {
      for (iterator it = begin(); it != end(); it++) delete &(*it);
      _head->_prev = _head->_next = _head;
   }

   void sort() const 
   { 
      if (size() < 2) return;
      for (iterator it = begin();
           it != end();
           it++)
      {
         for (iterator it2 = it;
              it2 != end();
              it2++)
         {
            if (it._node->_data > it2._node->_data)
            {
               T tmp = it._node->_data;
               it._node->_data = it2._node->_data;
               it2._node->_data = tmp;
            }
         }
      }
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
