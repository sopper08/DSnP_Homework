/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}

   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const { return _data[0]; }
   void insert(const Data& d) 
   { 
      size_t s = size() - 1;
      size_t t = ++s;
      _data.push_back(d);
      while (t > 1)
      {
         size_t p = (t - 1) / 2;
         if (_data[p] < d) break;
         _data[t] = _data[p];
         t = p;
      }
      _data[t] = d;
   }
   void delMin()
   {
      size_t p = 0, t = 2 * p + 1;
      size_t s = size() - 1;
      while (t <= s)
      {
         if (t < s) // has right tree
            if (_data[t+1] < _data[t]) ++t;
            if (_data[s] < _data[t]) break;
            _data[p] = _data[t];
            p = t;
            t = 2 * p + 1;
      }
      _data[p] = _data[s--];
   }
   void delData(size_t i)
   {
      size_t p = i, t = 2 * p + 1;
      size_t s = size() - 1;
      while (t <= s)
      {
         if (t < s) // has right tree
            if (_data[t+1] < _data[t]) ++t;
            if (_data[s] < _data[t]) break;
            _data[p] = _data[t];
            p = t;
            t = 2 * p + 1;
      }
      _data[p] = _data[s--];
   }

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
