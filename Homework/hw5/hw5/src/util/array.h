/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <algorithm>

using namespace std;

// NO need to implement class ArrayNode
//
template <class T>
class Array
{
public:
   // TODO: decide the initial value for _isSorted
   Array() : _data(0), _size(0), _capacity(0), _isSorted(false) {}
   ~Array() { delete []_data; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class Array;

   public:
      iterator(T* n= 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return (*this); }
      T& operator * () { return (*_node); }
      iterator& operator ++ () { _node = _node + 1; return (*this); }
      iterator operator ++ (int) { iterator tmp = *(this); _node = _node + 1; return tmp; }
      iterator& operator -- () { _node = _node - 1; return (*this); }
      iterator operator -- (int) { iterator tmp = *(this); _node = _node - 1; return tmp; }

      iterator operator + (int i) const { return (_node + i); }
      iterator& operator += (int i) { _node += i; return (*this); }

      iterator& operator = (const iterator& i) { _node = i._node; return (*this); }

      bool operator != (const iterator& i) const { return (_node != i._node); }
      bool operator == (const iterator& i) const { return (_node == i._node); }

   private:
      T*    _node;
   };

   // TODO: implement these functions
   iterator begin() const { return iterator(_data); }
   iterator end() const { return iterator(_data + _size); }
   bool empty() const { return (_size == 0); }
   size_t size() const { return _size; }

   T& operator [] (size_t i) { return _data[i]; }
   const T& operator [] (size_t i) const { return _data[i]; }

   void push_back(const T& x)
   { 
      if (_capacity == 0) resize(1);
      if ((_size + 1) == _capacity)
      {
         resize(2 * _capacity);
      }
      iterator new_node(end());
      *(new_node) = x;
      _size++;
      _isSorted = false;
   }
   void pop_front()
   {
      if (_size == 0) return;
      iterator b(begin());
      iterator l(--end());
      *b = *l;
      _size--;
      _isSorted = false;
   }
   void pop_back()
   { 
      if (_size == 0) return;
      _size--;
   }

   bool erase(iterator pos){
      iterator it(pos);
      iterator l(--end());
      *it = *l;
      _size--;
      _isSorted = false;
      
      return true;
   }

   bool erase(const T& x)
   { 
      if (empty()) return false;
      iterator it = find(x);
      // if (it == end()) return false;
      // if (it == begin()) { pop_front(); return true; }
      // if (it == end()) { pop_back(); return true; }
      iterator l(--end());
      *it = *l;
      _size--;
      _isSorted = false;

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

   void clear() 
   { 
      _data = 0;
      _size = 0;
      _capacity = 0;
      _isSorted = false;
   }

   // [Optional TODO] Feel free to change, but DO NOT change ::sort()
   void sort() const 
   { 
      if (_isSorted) return; 
      if (!empty()) ::sort(_data, _data+_size); 
   }

   // Nice to have, but not required in this homework...
   // void reserve(size_t n) { ... }
   void resize(size_t n)
   {
      T* t = new T[n];
      size_t s = size();
      for (size_t i = 0; i < s; ++i)
      {
         t[i] = _data[i];
      }
      _data = t;
      _capacity = n;
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   T*            _data;
   size_t        _size;       // number of valid elements
   size_t        _capacity;   // max number of elements
   mutable bool  _isSorted;   // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] Helper functions; called by public member functions
};

#endif // ARRAY_H
