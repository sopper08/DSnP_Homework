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

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
   stringstream ss;

   ss << getTypeStr() << "(" << getGateID() << ")";
   if (!_symbol.empty()) ss << "\"" << _symbol << "\"";
   ss << ", line " << getLineNo();

   cout << "==================================================" << endl;
   cout << left << setw(2) <<  "= ";
   cout << left << setw(46) << ss.str();
   cout << right << setw(2) << " =";
   cout << endl;
   cout << "==================================================" << endl;
}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
   static int intent = -1;
   static IdList faninLevelIdList;

   if (intent == -1) intent = level;

   cout << getTypeStr() << " " << getGateID();

   IdList::iterator pos = find(faninLevelIdList.begin(), faninLevelIdList.end(), getGateID());
   if ((pos != faninLevelIdList.end()) && (level > 0))
      { cout << " (*)" << endl; return; }
   cout << endl;

   int invIdx = 0;
   for (auto it = _faninGateList.begin(); it != _faninGateList.end(); ++it, ++invIdx)
   {
      int nLevel = level - 1;
      if (nLevel < 0 && intent != level) return;
      else if (nLevel < 0 && intent == level) { faninLevelIdList.clear(); intent = -1; return;}
      faninLevelIdList.push_back(getGateID());
      for (int i = 0; i < (intent - nLevel); ++i) cout << "  ";
      if (_faninGateInvPhaseList.at(invIdx)) cout << "!";
      (*it)->reportFanin(nLevel);
   }
   if (intent == level) { faninLevelIdList.clear(); intent = -1; }
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
   static int intent = -1;
   static IdList fanoutLevelIdList;

   if (intent == -1) intent = level;

   cout << getTypeStr() << " " << getGateID();

   IdList::iterator pos = find(fanoutLevelIdList.begin(), fanoutLevelIdList.end(), getGateID());
   if ((pos != fanoutLevelIdList.end()) && (level > 0))
      { cout << " (*)" << endl; return; }
   cout << endl;

   int invIdx = 0;
   for (auto it = _fanoutGateList.begin(); it != _fanoutGateList.end(); ++it, ++invIdx)
   {
      int nLevel = level - 1;
      if (nLevel < 0 && intent != level) return;
      else if (nLevel < 0 && intent == level) { fanoutLevelIdList.clear(); intent = -1; return;}
      fanoutLevelIdList.push_back(getGateID());
      for (int i = 0; i < (intent - nLevel); ++i) cout << "  ";
      if (_fanoutGateInvPhaseList.at(invIdx)) cout << "!";
      (*it)->reportFanout(nLevel);
   }
   if (intent == level) { fanoutLevelIdList.clear(); intent = -1; }
}

void
CirGate::dfsTraversal(GateList& dfsList)
{
   for (auto it = _faninGateList.begin(); it != _faninGateList.end(); it ++)
   {
      if (!(*it)->getTypeStr().compare("UNDEF")) continue;
      if (!(*it)->isGlobalRef())
      {
         (*it)->setToGlobalRef();
         (*it)->setActive();
         (*it)->dfsTraversal(dfsList);
         (*it)->unsetActive();
      }
   }
   dfsList.push_back(this);
}