/****************************************************************************
  FileName     [ p2Json.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Json and JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "p2Json.h"

using namespace std;

// Implement member functions of class Row and Table here
bool
Json::read(const string& jsonFile)
{
   char *buffer;
   int length;
   
   ifstream fin(jsonFile);
   if(!fin) return false;

   /* Get length of file. */
   fin.seekg(1, fin.end);
   length = fin.tellg();
   fin.seekg(0, fin.beg);
   buffer = new char[length];
   
   fin.read(buffer, length);
   _obj = _preProcessJsonFile(buffer);

   return true; // TODO
}

bool
Json::add(const string& keyAndValue)
{
   return true;
}

bool
Json::print()
{
   return true;
}

vector<JsonElem>
Json::_preProcessJsonFile(char *buffer)
{
   string data(buffer);
   int begIndex, endIndex;
   
   /* Remove space & "\t" charactor. */
   data.erase(remove(data.begin(), data.end(), ' '), data.end());
   cout << sizeof data << endl;
   data.erase(remove(data.begin(), data.end(), '\t'), data.end());
   cout << sizeof data << endl;
   data.erase(remove(data.begin(), data.end(), '\n'), data.end());
   begIndex = data.find_first_of("{");
   endIndex = data.find_first_of("}");
   // remove(data.begin(), str.end(), ' ');

   cout << begIndex << " " << endIndex << endl;
   cout << data << endl;
   cout << sizeof data << endl;
   vector <JsonElem> result;
   
   return result;
}

ostream&
operator << (ostream& os, const JsonElem& j)
{
   return (os << "\"" << j._key << "\" : " << j._value);
}

