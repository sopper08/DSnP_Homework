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
#include <iomanip>
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
Json::add(const string& command)
{
   // cout << command << endl;
   int secondSpaceIndex, commandLength;
   string key, value;
   
   commandLength = command.size();
   secondSpaceIndex = command.find_last_of(" ");
   key = command.substr(4, secondSpaceIndex-4);
   value = command.substr(secondSpaceIndex+1, commandLength-1);

   string keyAndValue("\"\":");
   keyAndValue.insert(3, value);
   keyAndValue.insert(1, key);

   _obj.push_back(_stringToJsonElem(keyAndValue));

   return true;
}

bool
Json::sum()
{
   if(_obj.size()==0)
   {
      cout << "Error: No element found!!" << endl;
      return true;
   }
   cout << "The summation of the values is: " << _sum << "." << endl;
   return true;
}


bool
Json::avg()
{
   if(_obj.size()==0)
   {
      cout << "Error: No element found!!" << endl;
      return true;
   }
   cout << "The average of the values is: " 
        << fixed << setprecision(1) << _avg << "." << endl;
   return true;
}

bool
Json::max()
{
   if(_obj.size()==0)
   {
      cout << "Error: No element found!!" << endl;
      return true;
   }
   cout << "The maximum element is: { " << _obj[_max_idx] << " }." << endl;
   return true;
}

bool
Json::min()
{
   if(_obj.size()==0)
   {
      cout << "Error: No element found!!" << endl;
      return true;
   }
   cout << "The minimum element is: { " << _obj[_min_idx] << " }." << endl;
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

      JsonElem elem = _stringToJsonElem(keyAndValue.substr(beg, end-beg+1));
      result.push_back(elem);
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
   _renewStatisticalData(value);

   JsonElem result(keyAndValue.substr(1, foundColon-2), value);

   return result;

}

void
Json::_renewStatisticalData(int value)
{
   _size ++;
   _sum += value;
   _avg = (float)_sum/(float)_size;
   if(_size!=1)
   {
      if(value > _max)
      {
         _max = value;
         _max_idx = _size-1;
      }
      if(value < _min)
      {
         _min = value;
         _min_idx = _size-1;
      } 
   }
   else
   {
      _max = value;
      _min = value;
      _max_idx = 0;
      _min_idx = 0;
   }

}

ostream&
operator << (ostream& os, const JsonElem& j)
{
   return (os << "\"" << j._key << "\" : " << j._value);
}

