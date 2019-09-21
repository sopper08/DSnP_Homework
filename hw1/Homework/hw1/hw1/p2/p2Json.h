/****************************************************************************
  FileName     [ p2Json.h]
  PackageName  [ p2 ]
  Synopsis     [ Header file for class Json JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#ifndef P2_JSON_H
#define P2_JSON_H

#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

class JsonElem
{
public:
   // TODO: define constructor & member functions on your own
   JsonElem() {}
   JsonElem(const string& k, int v): _key(k), _value(v) {}

   friend ostream& operator << (ostream&, const JsonElem&);

private:
   string  _key;   // DO NOT change this definition. Use it to store key.
   int     _value; // DO NOT change this definition. Use it to store value.
};

class Json
{
public:
   // TODO: define constructor & member functions on your own
   Json()
      :_size(0), _sum(0){}
   bool read(const string&);
   bool add(const string&);
   bool sum();
   bool avg();
   bool max();
   bool min();
   bool print();

private:
   vector<JsonElem>       _obj;  // DO NOT change this definition.
                                 // Use it to store JSON elements.
   vector<JsonElem> _preProcessJsonFile(char*);
   JsonElem _stringToJsonElem(const string&);
   void _renewStatisticalData(int);
   int _size;
   int _sum;
   float _avg;
   int _max;
   int _min;
   int _max_idx;
   int _min_idx;
};

#endif // P2_TABLE_H