/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>

using namespace std;

//---------------------
// Define HashSet class
//---------------------
// To use HashSet ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ==" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//
template <class Data>
class HashSet
{
public:
   HashSet(size_t b = 0) : _numBuckets(0), _buckets(0) { if (b != 0) init(b); }
   ~HashSet() { reset(); }

   // TODO: implement the HashSet<Data>::iterator
   // o An iterator should be able to go through all the valid Data
   //   in the Hash
   // o Functions to be implemented:
   //   - constructor(s), destructor
   //   - operator '*': return the HashNode
   //   - ++/--iterator, iterator++/--
   //   - operators '=', '==', !="
   //
   class iterator
   {
      friend class HashSet<Data>;

   public:
      iterator() { }
      iterator(typename vector<Data>::iterator i, const HashSet* s) { itr = i; set = s; }
      ~iterator() { }

      const Data& operator * () const { return *itr; }
      Data& operator * () { return *itr; }
      iterator& operator ++ () 
      {
         size_t bNum = set->bucketNum(*itr);
         // check self bucket first
         vector<Data>* v = (set->_buckets + bNum);
         if (next(itr) != v->end()) { itr = next(itr); return (*this); }

         // check self bucket first
         for (bNum+=1; bNum < set->_numBuckets; ++bNum)
         {
            v = (set->_buckets + bNum);
            if (!v->empty()) { itr = v->begin(); return (*this); }
         }

         itr = v->end();
         return (*this);
      }
      iterator operator ++ (int) { HashSet::iterator tmp = *this; ++this; return tmp; }
      iterator& operator -- ()
      {
         size_t bNum = set->bucketNum(*itr);
         // check self bucket first
         vector<Data>* v = (set->_buckets + bNum);
         if (itr != v->begin()) { itr = prev(itr); return (*this); }

         // check the foregoing buckets
         for (bNum-=1; bNum >= 0; --bNum)
         {
            v = (set->_buckets + bNum);
            if (!v->empty()) { itr = prev(v->end()); return (*this); }
         }
      }
      iterator operator -- (int) { HashSet::iterator tmp = *this; --this; return tmp; }

      iterator& operator = (const iterator& i) { itr = i; return (*this); }
      bool operator != (const iterator& i) const { return !((char*)(&(*itr)) == (char*)(&(*i))); }
      bool operator == (const iterator& i) const { return  ((char*)(&(*itr)) == (char*)(&(*i))); }

   private:
      const HashSet* set;
      typename vector<Data>::iterator itr;
   };

   void init(size_t b) { _numBuckets = b; _buckets = new vector<Data>[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   void clear() {
      for (size_t i = 0; i < _numBuckets; ++i) _buckets[i].clear();
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<Data>& operator [] (size_t i) { return _buckets[i]; }
   const vector<Data>& operator [](size_t i) const { return _buckets[i]; }

   // TODO: implement these functions
   //
   // Point to the first valid data
   iterator begin() const
   { 
      for (size_t i = 0; i < _numBuckets; ++i)
      {
         vector<Data>* v = (_buckets + i);
         if (!v->empty()) return iterator(v->begin(), this);
      }
      return iterator();
   }
   // Pass the end
   iterator end() const 
   {
      if (empty()) return begin();
      return iterator((_buckets + _numBuckets - 1)->end(), this);
   }
   // return true if no valid data
   bool empty() const 
   { 
      for (size_t i = 0; i < _numBuckets; ++i)
         if (!((_buckets + i)->empty())) return false;
      return true; 
   }
   // number of valid data
   size_t size() const 
   { 
      size_t s = 0; 
      for (size_t i = 0; i < _numBuckets; ++i)
         s += ((_buckets + i)->size());
      return s;
   }

   // check if d is in the hash...
   // if yes, return true;
   // else return false;
   bool check(const Data& d) const 
   {
      size_t bNum = bucketNum(d);
      auto v = (_buckets + bNum);
      auto it = v->begin();
      for (; it != v->end(); ++it)
         if ((*it) == d) return true;
      return false;
   }

   // query if d is in the hash...
   // if yes, replace d with the data in the hash and return true;
   // else return false;
   bool query(Data& d) const
   { 
      size_t bNum = bucketNum(d);
      auto v = (_buckets + bNum);
      auto it = v->begin();
      for (; it != v->end(); ++it)
         if ((*it) == d) { d = (*it); return true; }
      return false;
   }

   // update the entry in hash that is equal to d (i.e. == return true)
   // if found, update that entry with d and return true;
   // else insert d into hash as a new entry and return false;
   bool update(const Data& d) 
   {
      size_t bNum = bucketNum(d);
      auto v = (_buckets + bNum);
      auto it = v->begin();
      for (; it != v->end(); ++it)
         if ((*it) == d) { (*it) = d; return true; }
      
      insert(d);
      return false;
   }

   // return true if inserted successfully (i.e. d is not in the hash)
   // return false is d is already in the hash ==> will not insert
   bool insert(const Data& d)
   { 
      if (check(d)) return false;
      
      size_t bNum = bucketNum(d);
      (_buckets + bNum)->push_back(d);
      return true; 
   }

   // return true if removed successfully (i.e. d is in the hash)
   // return false otherwise (i.e. nothing is removed)
   bool remove(const Data& d) 
   {
      size_t bNum = bucketNum(d);
      auto v = (_buckets + bNum);
      auto it = v->begin();
      for (; it != v->end(); ++it)
         if ((*it) == d) { v->erase(it); return true; }

      return false; 
   }

private:
   // Do not add any extra data member
   size_t            _numBuckets;
   vector<Data>*     _buckets;

   size_t bucketNum(const Data& d) const {
      return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H
