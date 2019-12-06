/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool
CirMgr::readCircuit(const string& fileName)
{
   ifstream fin(fileName);
   vector<string> lines;
   string         line;

   while (getline(fin, line)) lines.push_back(line);

   // cout << "line[" << lineNo << "] read header: ";
   readHeader(lines.at(lineNo));
   // cout << endl;
   lineNo++;

   for (int i = 0; i < _header[1]; ++i, ++lineNo)
   {
      // cout << "line[" << lineNo << "] read input : ";
      readInput(lines.at(lineNo), lineNo);
      // cout << endl;
   }

   for (int i = 0; i < _header[3]; ++i, ++lineNo)
   {
      // cout << "line[" << lineNo << "] read output: ";
      readOutput(lines.at(lineNo), lineNo);
      // cout << endl;
   }

   for (int i = 0; i < _header[4]; ++i, ++lineNo)
   {
      // cout << "line[" << lineNo << "] read aig   : ";
      readAig(lines.at(lineNo), lineNo);
      // cout << endl;
   }

   connect();
   rconnect();
   checkFloatingAndNotUsedGates();
   genDFSList();

   return true;
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
   unsigned piSize  = _piList.size();
   unsigned poSize  = _poList.size();
   unsigned aigSize = _aigList.size();
   unsigned totalSize = piSize + poSize + aigSize;
   cout << endl;
   cout << "Circuit Statistics" << endl;
   cout << "==================" << endl;
   cout << "  " << left << setw(5) << "PI"    << right << setw(9) << piSize    << endl;
   cout << "  " << left << setw(5) << "PO"    << right << setw(9) << poSize    << endl;
   cout << "  " << left << setw(5) << "AIG"   << right << setw(9) << aigSize   << endl;
   cout << "------------------" << endl;
   cout << "  " << left << setw(5) << "Total" << right << setw(9) << totalSize << endl;
}

void
CirMgr::printNetlist() const
{
   cout << endl;
   int i = 0;
   for (auto it = _dfsList.begin(); it != _dfsList.end(); ++it)
   {
      cout << "[" << i << "] ";
      cout << left << setw(3) << (*it)->getTypeStr() << " " << (*it)->getGateID() << " ";
      GateList faninGateList = (*it)->getFaninGateList();
      vector<bool> invPhase = (*it)->getFaninGateInv();
      int invIdx = 0;
      for (auto itt = faninGateList.begin(); itt != faninGateList.end(); ++itt, ++invIdx)
      {
         if (!(*itt)->getTypeStr().compare("UNDEF")) cout << "*";
         if (invPhase.at(invIdx)) cout << "!";
         cout << (*itt)->getGateID() << " ";
      }
      cout << endl;
      ++i;
   }
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit: ";
   for (auto it = _piList.begin(); it != _piList.end(); ++it)
      cout << (*it)->getGateID() << " ";
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit: ";
   for (auto it = _poList.begin(); it != _poList.end(); ++it)
      cout << (*it)->getGateID() << " ";
   cout << endl;
}

void
CirMgr::printFloatGates() const
{
   if (!_floatingGates.empty())
   {
      cout << "Gates with floating fanin(s): ";
      for (auto it = _floatingGates.begin(); it != _floatingGates.end(); ++it)
         cout << (*it) << " ";
      cout << endl;
   }
   if (!_notUsedGates.empty())
   {
      cout << "Gates defined but not used  : ";
      for (auto it = _notUsedGates.begin(); it != _notUsedGates.end(); ++it)
         cout << (*it) << " ";
      cout << endl;
   }
}

void
CirMgr::writeAag(ostream& outfile) const
{
}

bool
CirMgr::readHeader(string line)
{
   line = line.substr(4);
   lexOptions(line, _header, 5);

   return true;
}

bool
CirMgr::readInput(string line, int lineNo)
{
   int gateId;
   myStr2Int(line, gateId);
   gateId >>= 1; // gateId should be /2
   CirGate *pi = new CirPiGate(gateId, lineNo);
   _piList.push_back(pi);
   _gateList[gateId] = pi;

   return true;
}

bool
CirMgr::readOutput(string line, int lineNo)
{
   int gateId = _header[0] + _poList.size() + 1;
   int faninId;
   bool invPhase;

   myStr2Int(line, faninId);
   invPhase = (faninId%2)==1;
   faninId = int(faninId/2);

   CirGate *po = new CirPoGate(gateId, lineNo, faninId, invPhase);
   _poList.push_back(po);
   _gateList[gateId] = po;

   return true;
}

bool
CirMgr::readAig(string line, int lineNo)
{
   vector<int> tokens;
   int gateId;
   int fanin0ID, fanin1ID;
   bool inv0, inv1;

   lexOptions(line, tokens, 3);

   gateId = tokens[0]/2;
   inv0 = tokens[1]%2==1;
   inv1 = tokens[2]%2==1;

   fanin0ID = int(tokens[1]/2);
   fanin1ID = int(tokens[2]/2);

   CirGate *aig = new CirAigGate(gateId, lineNo, fanin0ID, fanin1ID, inv0, inv1);
   _aigList.push_back(aig);
   _gateList[gateId] = aig;

   return true;
}

bool
CirMgr::connect()
{
   for (auto it = _gateList.begin(); it != _gateList.end(); ++it)
   {
      if (!((it->second)->getTypeStr().compare("PI")))
         continue;
      else if (!((it->second)->getTypeStr().compare("PO")))
      {
         // cout << "Po" << endl;
         IdList faninIdList = it->second->getFaninIdList();
         GateList g{_gateList[faninIdList[0]]};
         it->second->setFanin(g);
      }
      else if (!((it->second)->getTypeStr().compare("AIG")))
      {
         // cout << "Aig" << endl;
         IdList faninIdList = it->second->getFaninIdList();
         // CirGate *faninGate0, *faninGate1;
         auto it0 = _gateList.find(faninIdList[0]);
         if (it0 == _gateList.end()) { _gateList[faninIdList[0]] = new CirUndefGate(faninIdList[0]); }
         auto it1 = _gateList.find(faninIdList[1]);
         if (it1 == _gateList.end()) { _gateList[faninIdList[1]] = new CirUndefGate(faninIdList[1]); }

         GateList g{_gateList[faninIdList[0]], _gateList[faninIdList[1]]};
         it->second->setFanin(g);
      }
   }
   return true;
}

bool
CirMgr::rconnect()
{
   for (auto it = _gateList.begin(); it != _gateList.end(); ++it)
   {
      if (!(it->second->getTypeStr().compare("PI")))
         continue;
      else if ((!(it->second->getTypeStr().compare("PO"))) || (!(it->second->getTypeStr().compare("AIG"))))
      {
         GateList faninGateList = it->second->getFaninGateList();
         int invIdx = 0;
         for (auto itt = faninGateList.begin(); itt != faninGateList.end(); ++itt, ++invIdx)
            (*itt)->setFanout(it->second, it->second->getFaninGateInv()[invIdx]);
      }
   }
   return true;
}

bool
CirMgr::genDFSList()
{
   for (auto it = _poList.begin(); it != _poList.end(); it ++)
      (*it)->dfsTraversal(_dfsList);
   
   return true;
}

bool
CirMgr::checkFloatingAndNotUsedGates()
{
   for (auto it = _gateList.begin(); it != _gateList.end(); ++it)
   {
      // not used
      unsigned gateId = it->first;
      if (it->second->getFanoutGate() == it->second &&
          it->second->getTypeStr().compare("PO")) 
         _notUsedGates.push_back(gateId);
      GateList faninGateList = it->second->getFaninGateList();
      for (auto itt = faninGateList.begin(); itt != faninGateList.end(); ++itt)
         if (!(*itt)->getTypeStr().compare("UNDEF")) { _floatingGates.push_back(gateId); continue; }
   }
   return true;
}

bool
CirMgr::lexOptions
(const string& option, vector<int>& tokens, size_t nOpts) const
{
   string token;
   size_t n = myStrGetTok(option, token);
   while (token.size()) {
      int num;
      myStr2Int(token, num);
      tokens.push_back(num);
      n = myStrGetTok(option, token, n);
   }
   if (nOpts != 0) {
      if (tokens.size() < nOpts) {
         return false;
      }
      if (tokens.size() > nOpts) {
         return false;
      }
   }
   return true;
}
