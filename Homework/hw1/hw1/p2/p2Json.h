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
   string getKey() const { return _key; }
   int getValue() const { return _value; }

   friend ostream& operator << (ostream&, const JsonElem&);

private:
   string  _key;   // DO NOT change this definition. Use it to store key.
   int     _value; // DO NOT change this definition. Use it to store value.
};

class Json
{
public:
   // TODO: define constructor & member functions on your own
   Json(): _sum_value(0) {}
   bool read(const string&);
   void execute(const string&);

private:
   vector<JsonElem>       _obj;  // DO NOT change this definition.
                                 // Use it to store JSON elements.
   unordered_set<string>  _keySet;
   int _sum_value;
   size_t _min_pos;
   size_t _max_pos;

   bool _parseCmd(const string&, string&, vector<string>&);
   bool _checkArgsLength(const vector<string>&, const size_t&);
   string _trimString(const string&);
   void _add(JsonElem&);
   void _add(const string&, const string&);
   void _sum();
   void _ave();
   void _max();
   void _min();
   void _print();
};

#endif // P2_TABLE_H
