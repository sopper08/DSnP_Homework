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
      if ((!gateType.compare("PI")) | (!gateType.compare("PO"))) continue; // PI or PO, skip
      
      // AIG or UNDEF
      if ((find(_dfsList.begin(), _dfsList.end(), g)) != _dfsList.end()) continue; // not in dfsList, continue

      if (!gateType.compare("AIG"))
         cout << "Sweeping: AIG(" << g->getGateId() << ") removed..." << endl;
      if (!gateType.compare("UNDEF"))
         cout << "Sweeping: UNDEF(" << g->getGateId() << ") removed..." << endl;
      
      removedGateId.push_back(g->getGateId());
      
      // fanout gate remove this
      FanList fanout = g->fanList.getFanoutList();
      for (size_t i = 0; i < fanout.size(); ++i)
      {
         CirGate *fanoutGate = fanout[i]->getGate();
         fanoutGate->fanList.removeFaninGate(g);
      }

      // fanin gate remove this
      FanList fanin = g->fanList.getFaninList();
      for (size_t i = 0; i < fanin.size(); ++i)
      {
        CirGate *faninGate = fanin[i]->getGate();
        faninGate->fanList.removeFanoutGate(g);
      }
     
      _aigList.erase(remove(_aigList.begin(), _aigList.end(), g), _aigList.end());
      
   }
   for (size_t i = 0; i < removedGateId.size(); ++i)
      _gateList.erase(removedGateId[i]);
    
   resetFloatingGates();
}

// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
// UNDEF gates may be delete if its fanout becomes empty...
void
CirMgr::optimize()
{
   for (auto it = _poList.begin(); it != _poList.end(); ++it) optimize(*it);
   resetDFSParameters();
   resetDFSList();
   resetFloatingGates();
}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/

void
CirMgr::optimize(CirGate* g)
{
   FanList faninGateList = g->fanList.getFaninList();

   for (auto it = faninGateList.begin(); it != faninGateList.end(); ++it)
   {
      CirGate *g = (*it)->getGate();
      if (!g->getTypeStr().compare("UNDEF")) continue;
      if (!g->isGlobalRef())
      {
         g->setToGlobalRef();
         g->setActive();
         optimize(g);
         g->unsetActive();
      }
   }

   if (g->isAig())
   {
      FanList::const_iterator fanIt;
      if (g->fanList.isSameFaninGate(fanIt))
      {
         if (g->fanList.isSameFaninInv()) swapFanoutList(g, (*fanIt));
         else 
         {
            (*fanIt)->getGate()->fanList.removeFanoutGate(g);
            (*fanIt)->getGate()->fanList.removeFanoutGate(g); // remove 2 times, same Fanin
            swapFanoutList(g, new Fan(const0, false));
         }
      }
      else
      {
         FanList::iterator pos = g->fanList.findFaninGate(const0);
         if (pos != g->fanList.faninEnd()) // fanin has const0
         {
            if ((*pos)->getInv())
            {
               pos = g->fanList.findFaninGate(const0, true);
               swapFanoutList(g, (*pos));
            }
            else 
            {
               pos = g->fanList.findFaninGate(const0, true);
               (*pos)->getGate()->fanList.removeFanoutGate(g);
               swapFanoutList(g, new Fan(const0, false)); 
            }
         }
      }
   }
}

void
CirMgr::swapFanoutList(CirGate*& g, Fan* newFan)
{
   CirGate *newG = newFan->getGate(); bool newGInv = newFan->getInv();

   cout << "Simplifying: " << newG->getGateId() << " merging ";
   if (newGInv) cout << "!";
   cout << g->getGateId() << "..." << endl;

   FanList fanoutGateList = g->fanList.getFanoutList();
   for (size_t i = 0; i < fanoutGateList.size(); ++i)
   {
      bool oriInv = fanoutGateList[i]->getInv();
      bool newInv = newGInv ^ oriInv;
      
      Fan *f = new Fan(newG, newInv);
      fanoutGateList[i]->getGate()->fanList.replaceFanin(g, f);
      f->getGate()->fanList.replaceFanout(g, fanoutGateList[i]);
   }

   _aigList.erase(remove(_aigList.begin(), _aigList.end(), g), _aigList.end());
   _gateList.erase(g->getGateId());
}