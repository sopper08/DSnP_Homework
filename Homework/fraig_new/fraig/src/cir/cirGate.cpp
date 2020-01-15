/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include <algorithm>

#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

// TODO: Keep "CirGate::reportGate()", "CirGate::reportFanin()" and
//       "CirGate::reportFanout()" for cir cmds. Feel free to define
//       your own variables and functions.

extern CirMgr *cirMgr;

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
   stringstream ss;

   ss << this->getTypeStr() << "(" << this->getGateId() << ")";
   if (!_symbol.empty()) ss << "\"" << _symbol << "\"";
   ss << ", line " << getLineNo();

   cout << "================================================================================" << endl;
   cout << left << setw(2) <<  "= ";
   cout << left << ss.str() << endl;
   cout << "= FECs:" << endl;
   cout << "= Value: 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000" << endl;
   cout << "================================================================================" << endl;
}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
   
   static int    intent = -1;
   static IdList reportedGateIdList;
   unsigned      gId       = this->getGateId();
   string        gTypeStr  = this->getTypeStr();
   FanList       faninList = this->fanList.getFaninList();

   if (intent == -1) intent = level;

   cout << gTypeStr << " " << gId;

   IdList::iterator pos = find(reportedGateIdList.begin(), reportedGateIdList.end(), gId);
   if ((pos != reportedGateIdList.end()) && (level > 0)) { cout << " (*)" << endl; return; }
   cout << endl;

   for (size_t i = 0; i < faninList.size(); ++i)
   {
      int nLevel = level - 1;
      if (nLevel < 0 && intent != level) return;
      else if (nLevel < 0 && intent == level) { reportedGateIdList.clear(); intent = -1; return;}
      reportedGateIdList.push_back(gId);
      for (int i = 0; i < (intent - nLevel); ++i) cout << "  ";
      if (faninList[i]->getInv()) cout << "!";
      faninList[i]->getGate()->reportFanin(nLevel);
   }
   if (intent == level) { reportedGateIdList.clear(); intent = -1; }
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
   static int    intent = -1;
   static IdList reportedGateIdList;
   unsigned      gId        = this->getGateId();
   string        gTypeStr   = this->getTypeStr();
   FanList       fanoutList = this->fanList.getFanoutList();

   if (intent == -1) intent = level;

   cout << gTypeStr << " " << gId;

   IdList::iterator pos = find(reportedGateIdList.begin(), reportedGateIdList.end(), gId);
   if ((pos != reportedGateIdList.end()) && (level > 0)) { cout << " (*)" << endl; return; }
   cout << endl;

   for (size_t i = 0; i < fanoutList.size(); ++i)
   {
      int nLevel = level - 1;
      if (nLevel < 0 && intent != level) return;
      else if (nLevel < 0 && intent == level) { reportedGateIdList.clear(); intent = -1; return;}
      reportedGateIdList.push_back(gId);
      for (int i = 0; i < (intent - nLevel); ++i) cout << "  ";
      if (fanoutList[i]->getInv()) cout << "!";
      fanoutList[i]->getGate()->reportFanout(nLevel);
   }
   if (intent == level) { reportedGateIdList.clear(); intent = -1; }
}

unsigned
Fan::getGateId() const
{
   return _gate->getGateId();
}

size_t 
CirPiGate::simulate(vector<size_t>& signals)
{
   unsigned idx = this->getGateId() - 1;
   return signals[idx];
}

size_t 
CirPoGate::simulate(vector<size_t>& signals)
{
   assert(fanList.getFaninSize()==1);

   FanList fanin = fanList.getFaninList();
   size_t r = fanin[0]->getGate()->simulate(signals);
   if (fanin[0]->getInv()) return ~r;
   
   return r;
}

size_t 
CirAigGate::simulate(vector<size_t>& signals)
{
   assert(fanList.getFaninSize()==2);

   FanList fanin = fanList.getFaninList();
   size_t r0 = fanin[0]->getGate()->simulate(signals);
   size_t r1 = fanin[1]->getGate()->simulate(signals);
   r0 = (fanin[0]->getInv())?~r0:r0;
   r1 = (fanin[1]->getInv())?~r1:r1;

   return r0 & r1;
}

size_t 
CirConstGate::simulate(vector<size_t>& signals)
{
   return 0;
}

size_t 
CirUndefGate::simulate(vector<size_t>& signals)
{
   return 0;
}