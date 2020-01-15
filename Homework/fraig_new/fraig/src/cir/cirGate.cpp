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
#include <bitset>

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
   cout << "= FECs:";
   for (size_t i = 0; i < fecGrp.size(); ++i)
   {
      cout << " ";
      if (fecGrp[i]->getInv()) cout << "!";
      cout << fecGrp[i]->getGateId();
   }
   cout << endl;


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

bool 
CirPiGate::simulate()
{
   if (_firstSim) { _firstSim = false; return true; }
   return false;
}

bool 
CirPoGate::simulate()
{
   assert(fanList.getFaninSize()==1);

   FanList fanin = fanList.getFaninList();
   // fanin[0]->getGate()->simulate();
   size_t r = fanin[0]->getGate()->getSignal();
   r = (fanin[0]->getInv())?~r:r;

   // cout << bitset<64>(r) << endl;
   if(setSignal(r)) return true;
   
   return false;
}

bool
CirAigGate::simulate()
{
   assert(fanList.getFaninSize()==2);

   FanList fanin = fanList.getFaninList();
   // if ((!fanin[0]->getGate()->simulate()) &&
   //     (!fanin[1]->getGate()->simulate()) &&
   //     (!_firstSim)) { return false; }
   // fanin[0]->getGate()->simulate();
   // fanin[1]->getGate()->simulate();
   size_t r0 = fanin[0]->getGate()->getSignal();
   size_t r1 = fanin[1]->getGate()->getSignal();

   r0 = (fanin[0]->getInv())?~r0:r0;
   r1 = (fanin[1]->getInv())?~r1:r1;

   // _firstSim = false;
   if (setSignal(r0&r1)) {  return true; }

   return false;
}

bool 
CirConstGate::simulate()
{
   return false;
}

bool 
CirUndefGate::simulate()
{
   return false;
}