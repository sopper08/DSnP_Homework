/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <algorithm>
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
              << ": Cannot redefine const (" << errInt << ")!!" << endl;
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
   ifstream       fin(fileName);
   vector<string> lines;
   string         line;
   unsigned       end;

   /**************************
    *       read file        *
    **************************/
   while (getline(fin, line)) lines.push_back(line);
   if (lines.empty()) return false; // if no content in this file
   /********************
    *    read header   *
    ********************/
   if(!readHeader(lines.at(lineNo))) { return false; }
   ++lineNo;
   /********************
    *    read Input    *
    ********************/
   end =  lineNo + _header->_I; // lineNo: 1 ~ I
   for (; lineNo < end; ++lineNo) { readInput(lines[lineNo], lineNo); }
   /********************
    *    read Output   *
    ********************/
   end = lineNo + _header->_O; // lineNo: I+1 ~ I+O
   for (; lineNo < end; ++lineNo) { readOutput(lines[lineNo], lineNo); }
   /********************
    *     read Aig     *
    ********************/
   end = lineNo + _header->_A; // lineNo: I+O+1 ~ I+O+A
   for (; lineNo < end; ++lineNo) { readAig(lines[lineNo], lineNo); }
   /********************
    *    read Symbol   *
    ********************/
   while (lineNo < lines.size())
   {
      if (!lines.at(lineNo).compare("c")) break;
      readSymbol(lines[lineNo++]);
   }
   /**************************
    *        connect         *
    **************************/
   connect();
   rconnect();
   /**************************
    *  check floating gates  *
    **************************/
   resetFloatingGates();
   /**************************
    *           dfs          *
    **************************/
   resetDFSList();

   return true;
}

bool
CirMgr::readHeader(const string& s)
{
   string           tok;
   size_t           pos;
   vector<unsigned> headerValues;
   
   /* check header: aag */
   pos = myStrGetTok(s, tok);
   if (tok.compare("aag")) return false;
   
   /* store M, I, L, O, A */
   pos = myStrGetTok(s, tok, pos);
   while (tok.size())
   {
      int num;
      if (!myStr2Int(tok, num)) return false;
      headerValues.push_back(num);
      pos = myStrGetTok(s, tok, pos);
   }
   if (headerValues.size() != 5) return false;
   _header = new Header(headerValues);

   return true;
}

bool
CirMgr::readInput(const string& s, const int& lineNo)
{
   int gateId;

   myStr2Int(s, gateId);
   gateId >>= 1; // should be /2, according to aag format
   CirGate *pi = new CirPiGate(gateId, lineNo);
   /* renew _piList, _gateList */
   _piList.push_back(pi);
   _gateList.insert({gateId, pi});

   return true;
}

bool
CirMgr::readOutput(const string& s, const int& lineNo)
{
   int gateId = (_header->_M + 1) + _poList.size(); // number id from (M+1)
   int fanin;

   myStr2Int(s, fanin);
   CirGate *po = new CirPoGate(gateId, lineNo, fanin);
   /* renew _poList, _gateList */
   _poList.push_back(po);
   _gateList.insert({gateId, po});

   return true;
}

bool
CirMgr::readAig(const string& s, const int& lineNo)
{
   int gateId;
   int fanin0, fanin1;
   string tok; unsigned pos;

   /* read gateId */
   pos = myStrGetTok(s, tok);
   myStr2Int(tok, gateId);
   gateId = gateId/2;

   /* read fanin */
   pos = myStrGetTok(s, tok, pos);
   myStr2Int(tok, fanin0);
   pos = myStrGetTok(s, tok, pos);
   myStr2Int(tok, fanin1);

   CirGate *aig = new CirAigGate(gateId, lineNo, fanin0, fanin1);
   /* renew _aigList, _gateList */
   _aigList.push_back(aig);
   _gateList.insert({gateId, aig});

   return true;
}

bool
CirMgr::readSymbol(string& s)
{
   string gType;
   int order;
   string tok; unsigned pos;
   
   /* get gate type */
   gType = s.at(0);
   s = s.substr(1);
   /* get order     */
   pos = myStrGetTok(s, tok);
   myStr2Int(tok, order);
   /* get symbol    */
   pos = myStrGetTok(s, tok, pos);

   if (!gType.compare("i")) _piList[order]->setSymbol(tok);
   else if (!gType.compare("o")) _poList[order]->setSymbol(tok);

   return true;
}

bool
CirMgr::connect()
{
   /**************************
    *      connect aig       *
    **************************/
   for (auto it = _aigList.begin(); it != _aigList.end(); ++it)
   {
      /********************
       *    cvt to gId    *
       ********************/
      unsigned fanin0Id = static_cast<CirAigGate*>(*it)->oriFanin0;
      unsigned fanin1Id = static_cast<CirAigGate*>(*it)->oriFanin1;
      bool inv0 = (fanin0Id%2)==1; fanin0Id /= 2;
      bool inv1 = (fanin1Id%2)==1; fanin1Id /= 2;
      /********************
       *    cnt fanin0    *
       *       &fanin1    * 
       ********************/
      GateMap::iterator fanin0Pos, fanin1Pos;
      if (!findGate(_gateList, fanin0Id, fanin0Pos)) { return false; }
      if (!findGate(_gateList, fanin1Id, fanin1Pos)) { return false; }

      vector<Pair_CirGateAndInv> gs{Pair_CirGateAndInv((*fanin0Pos).second, inv0), 
                                    Pair_CirGateAndInv((*fanin1Pos).second, inv1)};
      (*it)->fanList.setFanin(gs);
   }
   /**************************
    *      connect Po        *
    **************************/
   for (auto it = _poList.begin(); it != _poList.end(); ++it)
   {
      /********************
       *    cvt to gId    *
       ********************/
      unsigned faninId = static_cast<CirPoGate*>(*it)->oriFanin;
      bool inv = (faninId%2)==1; faninId /= 2;
      /********************
       *    cnt fanin     *
       ********************/
      GateMap::iterator faninPos;
      if (!findGate(_gateList, faninId, faninPos)) { return false; }
      
      vector<Pair_CirGateAndInv> gs{Pair_CirGateAndInv((*faninPos).second, inv)};
      (*it)->fanList.setFanin(gs);
   }

   return true;
}

bool
CirMgr::rconnect()
{
   for (auto it = _gateList.begin(); it != _gateList.end(); ++it)
   {
      CirGate *g = it->second;
      string typeStr = g->getTypeStr();
      if ((!typeStr.compare("PO")) || (!typeStr.compare("AIG"))) // choose PO & AIG
      {
         FanList faninList = g->fanList.getFaninList();
         for (size_t i = 0; i < faninList.size(); ++i)
            faninList[i]->getGate()->fanList.addFanout(g, faninList[i]->getInv());
      }
   }

   return true;
}

void
CirMgr::resetFloatingGates()
{
   _floatingFaninIdList.clear(); _defButNotUsedIdList.clear();
   
   for (size_t i = 0; i < _piList.size(); ++i) // pi
      if (_piList[i]->fanList.getFanoutSize() < 1) _defButNotUsedIdList.push_back(_piList[i]->getGateId());

   for (size_t i = 0; i < _aigList.size(); ++i) // aig
   {
      if (_aigList[i]->fanList.getFanoutSize() < 1) _defButNotUsedIdList.push_back(_aigList[i]->getGateId());

      GateList faninGateList = _aigList[i]->fanList.getFaninGateList();
      if (existUNDEFGate(faninGateList)) _floatingFaninIdList.push_back(_aigList[i]->getGateId());
   }

   for (size_t i = 0; i < _poList.size(); ++i) // po
   {
      GateList faninGateList = _poList[i]->fanList.getFaninGateList();
      if (existUNDEFGate(faninGateList)) _floatingFaninIdList.push_back(_poList[i]->getGateId());
   }
}

bool
CirMgr::existUNDEFGate(GateList& gL)
{
   for (size_t i = 0; i < gL.size(); ++i)
      if (!(gL[i]->getTypeStr().compare("UNDEF"))) return true;
   return false;
}

void
CirMgr::resetDFSList()
{
   _dfsList.clear();

   for (auto it = _poList.begin(); it != _poList.end(); ++it)
      (*it)->dfsTraversal(_dfsList);
   
   resetDFSParameters();
}

void
CirMgr::resetDFSParameters()
{
   for (auto it = _gateList.begin(); it != _gateList.end(); ++it)
   {
      (*it).second->unsetToGlobalRef(); // reset parameters dfs needed
      (*it).second->unsetActive();
   }
}

bool
CirMgr::findGate(GateMap& gList, unsigned id, GateMap::iterator& pos, bool addIt)
{
   pos = gList.find(id);
   if (pos == gList.end())
   {
      if (addIt) 
      {
         CirGate *f = new CirUndefGate(id);
         pair<unsigned, CirGate*> p(id, f);
         pair<GateMap::iterator, bool> result = gList.insert(p);
         if (result.second) pos = result.first;
         else { return false; }
      }
      else { return false; }
   }

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
   cout << endl;
   cout << "Circuit Statistics" << endl;
   cout << "==================" << endl;
   cout << "  " << left << setw(5) << "PI"    << right << setw(9) << _piList.size()    << endl;
   cout << "  " << left << setw(5) << "PO"    << right << setw(9) << _poList.size()    << endl;
   cout << "  " << left << setw(5) << "AIG"   << right << setw(9) << _aigList.size()   << endl;
   cout << "------------------" << endl;
   cout << "  " << left << setw(5) << "Total" << right << setw(9);
   cout << (_piList.size() + _poList.size() + _aigList.size()) << endl;
}

void
CirMgr::printNetlist() const
{
   cout << endl;
   for (unsigned i = 0, n = _dfsList.size(); i < n; ++i) {
      cout << "[" << i << "] ";
      _dfsList[i]->printGate();
   }
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit:";
   for (auto it = _piList.begin(); it != _piList.end(); ++it)
      cout << " " << (*it)->getGateId();
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit:";
   for (auto it = _poList.begin(); it != _poList.end(); ++it)
      cout << " " << (*it)->getGateId();
   cout << endl;
}

void
CirMgr::printFloatGates() const
{
   if (!_floatingFaninIdList.empty())
   {
      cout << "Gates with floating fanin(s):";
      for (auto it = _floatingFaninIdList.begin(); it != _floatingFaninIdList.end(); ++it)
         cout << " " << (*it);
      cout << endl;
   }
   if (!_defButNotUsedIdList.empty())
   {
      cout << "Gates defined but not used  :";
      for (auto it = _defButNotUsedIdList.begin(); it != _defButNotUsedIdList.end(); ++it)
         cout << " " << (*it);
      cout << endl;
   }
}


void
CirMgr::printFECPairs() const
{
   for (size_t i = 0; i < _fecGrps.size(); ++i)
   {
      cout << "[" << i << "]";
      for (size_t j = 0; j < _fecGrps[i].size(); ++j)
      {
         cout << " ";
         if (_fecGrps[i][j]->getInv()) cout << "!";
         cout << _fecGrps[i][j]->getGateId();
      }
      cout << endl;
   }
}

void
CirMgr::writeAag(ostream& outfile) const
{
}

void
CirMgr::writeGate(ostream& outfile, CirGate *g) const
{
}

