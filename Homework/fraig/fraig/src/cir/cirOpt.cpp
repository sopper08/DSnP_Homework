/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir optimization functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

#include <algorithm>
#include <vector>

using namespace std;

// TODO: Please keep "CirMgr::sweep()" and "CirMgr::optimize()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
// Remove unused gates
// DFS list should NOT be changed
// UNDEF, float and unused list may be changed
// be changed: _aigList, _floatingGates, _notUsedGates, _gateList
void
CirMgr::sweep()
{
   IdList removedGateId;

   auto it = _gateList.begin();
   for (++it; it != _gateList.end(); ++it) // skip const 0 (gateId: 0)
   {
      CirGate *g = it->second;
      string gateType = g->getTypeStr();
      // PI or PO
      if ((!gateType.compare("PI")) | (!gateType.compare("PO"))) continue; // be PI or PO, continue
      
      // AIG or UNDEF
      if ((_dfsSet.find(g)) != _dfsSet.end()) continue; // not in dfsList, continue

      if (!gateType.compare("AIG"))
         cout << "Sweeping: AIG(" << g->getGateID() << ") removed..." << endl;
      if (!gateType.compare("UNDEF"))
         cout << "Sweeping: UNDEF(" << g->getGateID() << ") removed..." << endl;
      
      removedGateId.push_back(g->getGateID());
      
      // fanout gate remove this
      GateList fanout = g->getFanoutGateList();
      for (size_t i = 0; i < fanout.size(); ++i)
      {
         GateList gFanin = fanout[i]->getFaninGateList();
         for (size_t j = 0; j < gFanin.size(); ++j)
            if (gFanin[j] == g) { fanout[i]->removeFanin(j); break; }
      }

      // fanin gate remove this
      GateList fanin = g->getFaninGateList();
      for (size_t i = 0; i < fanin.size(); ++i)
      {
         GateList gFanout = fanin[i]->getFanoutGateList();
         for (size_t j = 0; j < gFanout.size(); ++j)
            if (gFanout[j] == g) { fanin[i]->removeFanout(j); break; }
      }
     
      _aigList.erase(remove(_aigList.begin(), _aigList.end(), g), _aigList.end());
      
   }
   for (size_t i = 0; i < removedGateId.size(); ++i)
      _gateList.erase(removedGateId[i]);

   checkFloatingAndNotUsedGates();
   
}

// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
// UNDEF gates may be delete if its fanout becomes empty...
void
CirMgr::optimize()
{
   for (auto it = _poList.begin(); it != _poList.end(); ++it)
   {
      optimize(*it);
   }
}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/

void
CirMgr::optimize(CirGate* g)
{
   GateList faninGateList = g->getFaninGateList();
   for (auto it = faninGateList.begin(); it != faninGateList.end(); ++it)
   {
      if (!(*it)->getTypeStr().compare("UNDEF")) continue;
      if (!(*it)->isGlobalRef())
      {
         (*it)->setToGlobalRef();
         (*it)->setActive();
         optimize(*it);
         (*it)->unsetActive();
      }
   }

   if (g->isAig())
   {
      size_t gIdx = 0;
      bool existConst0 = false, inv = false;
      bool sameFanin = g->isSameFanin(gIdx, const0, existConst0);
      if (sameFanin)
      {
         CirGate *cG = g->getFaninGateList()[gIdx];
         if (g->isSameFaninInvPhase(inv)) { swapFanoutGateList(cG, inv, g); }
         else { swapFanoutGateList(const0, false, g); }
      }
      else
      {
         if (existConst0)
         {
            bool const0Inv = g->getFaninGateInv()[1 - gIdx];
            if (!const0Inv) gIdx = 1 - gIdx; // change gIdx to 0
            CirGate* cG = g->getFaninGateList()[gIdx];
            bool cGInv = g->getFaninGateInv()[gIdx];
            swapFanoutGateList(cG, cGInv, g);
         }
      }
   }
}

void
CirMgr::swapFanoutGateList(CirGate* newG, bool newInv, CirGate* rG)
{
   cout << "Simplifying: " << newG->getGateID() << " merging ";
   if (newInv) cout << "!";
   cout << rG->getGateID() << "..." << endl;

   GateList fanoutGateList = rG->getFanoutGateList();
   for (size_t i = 0; i < fanoutGateList.size(); ++i)
   {
      bool oriInv = fanoutGateList[i]->getFaninGateInv(rG);
      newInv = newInv ^ oriInv;
      fanoutGateList[i]->removeFanin(rG);
      fanoutGateList[i]->setFanin(newG, newInv);
   }
}