/****************************************************************************
  FileName     [ dbJson.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Json member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <cmath>
#include <string>
#include <algorithm>
#include "dbJson.h"
#include "util.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream&
operator << (ostream& os, const DBJsonElem& j)
{
   os << "\"" << j._key << "\" : " << j._value;
   return os;
}

istream& operator >> (istream& is, DBJson& j)
{
   // TODO: to read in data from Json file and store them in a DB 
   // - You can assume the input file is with correct JSON file format
   // - NO NEED to handle error file format
   assert(j._obj.empty());
   while(true)
   {
      string buffer, tmp;
      string key;
      int value;
      size_t kvPairPtr;
      size_t begOfValueStr, endOfValueStr;

      getline(is, buffer);
      if(!(buffer.find('{')==string::npos)) { continue; }
      if(!(buffer.find('}')==string::npos)) { break; }
      if((buffer.find('\"')==string::npos)) { continue; }

      kvPairPtr = buffer.find_first_of('\"');
      kvPairPtr = myStrGetTok(buffer, tmp, kvPairPtr, '\"');
      key = tmp;
      kvPairPtr = buffer.find_first_of(" : ", kvPairPtr);
      kvPairPtr = myStrGetTok(buffer, tmp, kvPairPtr+3, ',');
      begOfValueStr = tmp.find_first_not_of(' ');
      endOfValueStr = tmp.find_first_of(' ');
      myStr2Int(tmp.substr(begOfValueStr, endOfValueStr-begOfValueStr), value);
      
      DBJsonElem elem(key, value);
      j._obj.push_back(elem);

      if(buffer=="}") { break; }
   }
   return is;
}

ostream& operator << (ostream& os, const DBJson& j)
{
   // TODO
   if(j.size()==0)
   {
      cerr << "Error: DB is not created yet!!" << endl;
   }
   cout << "{" << endl;
   for(auto it = j._obj.begin();
       it != j._obj.end();
       it++)
   {
      cout << "  " << *it << endl;
   }
   cout << "}" << endl;
   return os;
}

/**********************************************/
/*   Member Functions for class DBJsonElem    */
/**********************************************/
/*****************************************/
/*   Member Functions for class DBJson   */
/*****************************************/
void
DBJson::reset()
{
   // TODO
}

// return false if key is repeated
bool
DBJson::add(const DBJsonElem& elm)
{
   // TODO
   for(auto it = _obj.begin();
       it != _obj.end();
       it ++)
   {
      if(elm.key()==it->key()) 
      {
         cerr << "Error: Element with key \"" << elm.key() << "\" already exists!!" << endl;
         return false;
      }
   }
   _obj.push_back(elm);
   return true;
}

// return NAN if DBJson is empty
float
DBJson::ave() const
{
   // TODO
   float sum = 0;

   if(this->size()==0) { cerr << "Error: The average of the DB is nan." << endl;}
   for(auto it = _obj.begin();
       it != _obj.end();
       it ++)
   {
      sum += it->value();
   }

   return sum/(float)_obj.size();
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::max(size_t& idx) const
{
   // TODO
   if(_obj.size()==0)
   {
      idx = _obj.size();
      int maxN = INT_MIN;
      return maxN;
   }
   else
   {
      for(auto it = _obj.begin();
          it != _obj.end();
          it ++)
      {
         if(it->value() > _obj.at(idx).value()) { idx = distance(_obj.begin(), it); } 
      }
   }

   return _obj.at(idx).value();
}

// If DBJson is empty, set idx to size() and return INT_MAX
int
DBJson::min(size_t& idx) const
{
   // TODO
   if(_obj.size()==0)
   {
      idx = _obj.size();
      int minN = INT_MAX;
      return  minN;
   }
   else
   {
      for(auto it = _obj.begin();
          it != _obj.end();
          it ++)
      {
         if(it->value() < _obj.at(idx).value()) { idx = distance(_obj.begin(), it); } 
      }
   }

   return _obj.at(idx).value();
}

void
DBJson::sort(const DBSortKey& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

void
DBJson::sort(const DBSortValue& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

// return 0 if empty
int
DBJson::sum() const
{
   // TODO
   if(_obj.size()==0) { return 0; }
   int s = 0;
   for(auto it = _obj.begin();
         it != _obj.end();
         it ++)
   {
      s += it->value();
   }
   return s;
}
