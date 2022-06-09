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
#include <math.h>


using namespace std;

// Implement member functions of class Row and Table here
bool
Json::read(const string& jsonFile)
{
   // TODO: implement this function.
   stringstream ss;
   string s;
   size_t left_curly_braces_pos, right_curly_braces_pos, colon_pos, comma_pos;
   ifstream ifs(jsonFile);

   if (!ifs) return false;
   ss << ifs.rdbuf();
   s = ss.str();
   
   left_curly_braces_pos = s.find_first_of('{');
   right_curly_braces_pos = s.find_last_of('}');
   if (left_curly_braces_pos == string::npos || right_curly_braces_pos == string::npos) return false;
   s = s.substr(left_curly_braces_pos + 1, right_curly_braces_pos - left_curly_braces_pos - 1);
   
   comma_pos = left_curly_braces_pos+1;
   colon_pos = s.find_first_of(':', comma_pos);
   while (colon_pos != string::npos) {
      size_t left_double_quote, right_double_quote;
      string key;
      int value;

      left_double_quote = s.find_first_of('"', comma_pos);
      right_double_quote = s.find_first_of('"', left_double_quote + 1);
      key = _trimString(s.substr(left_double_quote + 1, right_double_quote-left_double_quote-1));
      
      comma_pos = s.find_first_of(',', comma_pos + 1);
      if (comma_pos == string::npos) comma_pos = right_curly_braces_pos;
      
      value = stoi(s.substr(colon_pos + 1, comma_pos - colon_pos - 1));
      
      JsonElem elem(key, value);
      _add(elem);

      colon_pos = s.find_first_of(':', comma_pos);
   }
   ifs.close();
   return true;
}

void 
Json::execute(const string& line)
{
   string action;
   vector<string> args;

   if (!_parseCmd(line, action, args)) return;

   if (action == "SUM") {
      if (!_checkArgsLength(args, 0)) return;
      _sum();
   }
   else if (action == "AVE") {
      if (!_checkArgsLength(args, 0)) return;
      _ave();
   }
   else if (action == "MAX") {
      if (!_checkArgsLength(args, 0)) return;
      _max();
   }
   else if (action == "MIN") {
      if (!_checkArgsLength(args, 0)) return;
      _min();
   }
   else if (action == "PRINT") {
      if (!_checkArgsLength(args, 0)) return;
      _print();
   }
   else if (action == "ADD") {
      if (!_checkArgsLength(args, 2)) return;
      _add(args[0], args[1]);
   }
   else {
      cout << "Error: unknown command: " << "\"" << action << "\"" << endl;
   }
}

bool
Json::_parseCmd(const string& line, string& action, vector<string>& args)
{
   size_t pos;
   string _line;

   _line = _trimString(line);
   if (_line.empty()) return false;

   
   pos = _line.find_first_of(' ');
   if (pos == string::npos) {
      action = line;
      return true;
   } else {
      action = _line.substr(0, pos);
      _line = _line.substr(pos + 1);
   }

   pos = _line.find_first_not_of(' ');
   while (pos != string::npos) {
      size_t end = _line.find_first_of(' ', pos);
      if (end == string::npos) end = _line.size();
      args.push_back(_line.substr(pos, end - pos));
      pos = _line.find_first_not_of(' ', end);
   }

   return true;
}

bool
Json::_checkArgsLength(const vector<string>& args, const size_t& length)
{
   if (args.size() > length) {
      cerr << "Error: Extra argument \"";
      for (size_t i = length; i < args.size(); ++i) {
         cerr << args[i];
         if (i != args.size() - 1) cerr << " ";
      }
      cerr << "\"!!" << endl;
      return false;
   }
   else if (args.size() < length) {
      cerr << "Error: Missing argument";
      if (args.size()>0) {
         cerr << " after \"" << args[args.size()-1] << "\"";
      }
      cerr << "!!" << endl;
      return false;
   }
   return true;
}

void
Json::_add(JsonElem& je)
{
   if (_keySet.find(je.getKey())!=_keySet.end()) {
      cerr << "Error: Key " << "\"" << je.getKey() << "\" is repeated!!" << endl;
      return;
   }

   if (_obj.empty()){
      _max_pos = 0;
      _min_pos = 0;
   } else {
      if (je.getValue() > _obj[_max_pos].getValue()) {
         _max_pos = _obj.size();
      } 
      if (je.getValue() < _obj[_min_pos].getValue()) {
         _min_pos = _obj.size();
      }
   }
   _obj.push_back(je);
   _keySet.insert(je.getKey());
   _sum_value += je.getValue();
}

void
Json::_add(const string& key, const string& value)
{
   int _value;

   if (!isdigit(value[0]) && (value[0]!='-')) {
      cerr << "Error: Illegal argument \"";
      cerr << value << "\"!!" << endl;
      return;
   }
   for (size_t i = 1; i < value.size(); ++i){
      if (!isdigit(value[i])) {
         cerr << "Error: Illegal argument \"";
         cerr << value << "\"!!" << endl;
         return;
      }
   }
   _value = stoi(value);
   JsonElem je(key, _value);
   _add(je);
}

void
Json::_sum()
{
   if(_obj.size()==0)
   {
      cerr << "Error: No element found!!" << endl;
      return;
   }
   cout << "The summation of the values is: " << _sum_value << "." << endl;
}


void
Json::_ave()
{
   if(_obj.size()==0)
   {
      cerr << "Error: No element found!!" << endl;
      return;
   }
   bool is_neg = (_sum_value < 0);
   float ave = float(int(round(float(abs(_sum_value))/float(_obj.size())*10.0)))/10.0;
   if (is_neg) ave = -ave;
   cout << "The average of the values is: " 
        << fixed << setprecision(1) << ave << "." << endl;
}

void
Json::_max()
{
   if(_obj.size()==0)
   {
      cerr << "Error: No element found!!" << endl;
      return;
   }
   cout << "The maximum element is: { " << _obj[_max_pos] << " }." << endl;
}

void
Json::_min()
{
   if(_obj.size()==0)
   {
      cerr << "Error: No element found!!" << endl;
      return;
   }
   cout << "The minimum element is: { " << _obj[_min_pos] << " }." << endl;
}

void
Json::_print()
{
   cout << "{" << endl;
   for (size_t i = 0; i < _obj.size(); ++i) {
      cout << "  " << _obj[i];
      if (i!=_obj.size()-1) cout << ",";
      cout << endl;
   }
   cout << "}" << endl;
}


string
Json::_trimString(const string& str)
{
   size_t first = str.find_first_not_of(' ');
   size_t last = str.find_last_not_of(' ');
   return str.substr(first, (last - first + 1));
}

ostream&
operator << (ostream& os, const JsonElem& j)
{
   return (os << "\"" << j._key << "\" : " << j._value);
}