/****************************************************************************
  FileName     [ test.cpp ]
  PackageName  [ test ]
  Synopsis     [ Test program for simple database db ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "util.h"
#include "dbJson.h"

using namespace std;

extern DBJson dbjson;

class CmdParser;
CmdParser* cmdMgr = 0; // for linking purpose

int
main(int argc, char** argv)
{
   if (argc != 2) {  // testdb <jsonfile>
      cerr << "Error: using testdb <jsonfile>!!" << endl;
      exit(-1);
   }

   ifstream inf(argv[1]);

   if (!inf) {
      cerr << "Error: cannot open file \"" << argv[1] << "\"!!\n";
      exit(-1);
   }

   if (dbjson) {
      cout << "Table is resetting..." << endl;
      dbjson.reset();
   }
   
   if (!(inf >> dbjson)) {
      cerr << "Error in reading JSON file!!" << endl;
      exit(-1);
   }
   // TODO
   // Insert what you want to test here by calling DBJson's member functions

   cout << "========================" << endl;
   cout << " Ave                    "  << endl;
   cout << "========================" << endl;
   cout << dbjson.ave() << endl;
   cout << dbjson << endl;

   cout << "========================" << endl;
   cout << " Print JSON object" << endl;
   cout << "========================" << endl;
   cout << dbjson << endl;

   cout << "========================" << endl;
   cout << " Append an element"       << endl;
   cout << "========================" << endl;
   dbjson.add(DBJsonElem("D1", -92));
   cout << dbjson << endl;

   cout << "========================" << endl;
   cout << " Append an repeated key"  << endl;
   cout << "========================" << endl;
   dbjson.add(DBJsonElem("Ric", -92));
   cout << dbjson << endl;

   cout << "========================" << endl;
   cout << " Ave                    "  << endl;
   cout << "========================" << endl;
   cout << dbjson.ave() << endl;
   cout << dbjson << endl;

   cout << "========================" << endl;
   cout << " Max                    "  << endl;
   cout << "========================" << endl;
   size_t idx = 0;
   cout << dbjson.max(idx) << endl;
   cout << dbjson << endl;

   cout << "========================" << endl;
   cout << " Min                    "  << endl;
   cout << "========================" << endl;
   idx = 0;
   cout << dbjson.min(idx) << endl;
   cout << dbjson << endl;

   cout << "========================" << endl;
   cout << " Sum                    " << endl;
   cout << "========================" << endl;
   idx = 0;
   cout << dbjson.sum() << endl;
   cout << dbjson << endl;

   return 0;
}
