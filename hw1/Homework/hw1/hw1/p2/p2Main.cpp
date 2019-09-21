/****************************************************************************
  FileName     [ p2Main.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define main() function ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include "p2Json.h"

using namespace std;

int main()
{
   Json json;

   // Read in the csv file. Do NOT change this part of code.
   string jsonFile;
   cout << "Please enter the file name: ";
   cin >> jsonFile;
   if (json.read(jsonFile))
      cout << "File \"" << jsonFile << "\" was read in successfully." << endl;
   else {
      cerr << "Failed to read in file \"" << jsonFile << "\"!" << endl;
      exit(-1); // jsonFile does not exist.
   }

   // TODO read and execute commands
   cout << "Enter command: ";
   while (true) {
      string command;

      getline(cin, command);

      if(!command.compare("PRINT")) json.print();
      else if(!command.compare(0, 3, "ADD")) json.add(command);
      else if(!command.compare("SUM")) json.sum();
      else if(!command.compare("AVG")) json.avg();
      else if(!command.compare("MAX")) json.max();
      else if(!command.compare("MIN")) json.min();
      else if(!command.compare("EXIT")) break;
      else continue;

      cout << "Enter command: ";
   }
}
