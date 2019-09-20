/****************************************************************************
  FileName     [ p2Json.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Json and JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
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
   cout << "{" << endl;
   for(vector<JsonElem>::iterator it = _obj.begin();
       it != _obj.end();
       it++)
       cout << "  " << *it << endl;
   cout << "}" << endl;
   return true;
}

vector<JsonElem>
Json::_preProcessJsonFile(char *buffer)
{
   string data(buffer);
   string keyAndValue;
   vector<string> keyAndValue_array;
   int begIndex, length;
   size_t foundComma, prefoundComma;
   vector <JsonElem> result;
   
   /* Remove space & "\t" & "\n" charactor. */
   data.erase(remove(data.begin(), data.end(), ' '), data.end());
   data.erase(remove(data.begin(), data.end(), '\t'), data.end());
   data.erase(remove(data.begin(), data.end(), '\n'), data.end());
   
   /* Remove "{" & "}" charactor */
   begIndex = data.find_first_of("{");
   length = data.find_first_of("}") - begIndex - 1;
   keyAndValue = data.substr(begIndex+1, length);
   keyAndValue.insert(0, ",");

   /* Create vector of JsonElem */
   foundComma = keyAndValue.find(",", 1);

   if(foundComma==string::npos) return result;

   prefoundComma = 0;
   while(prefoundComma!=string::npos){
      int beg, end;

      beg = prefoundComma + 1;
      if(foundComma!=string::npos) end = foundComma - 1;
      else end = length;

      result.push_back(_stringToJsonElem(keyAndValue.substr(beg, end-beg+1)));
      prefoundComma = foundComma;
      foundComma = keyAndValue.find(",", foundComma+1);
   }
   
   return result;
}

JsonElem
Json::_stringToJsonElem(const string& keyAndValue)
{
   size_t foundColon;
   int value;

   foundColon = keyAndValue.find(":");
   istringstream iss(keyAndValue.substr(foundColon+1, sizeof(keyAndValue)-foundColon));
   iss >> value;

   JsonElem result(keyAndValue.substr(1, foundColon-2), value);
   return result;
}

ostream&
operator << (ostream& os, const JsonElem& j)
{
   return (os << "\"" << j._key << "\" : " << j._value);
}

