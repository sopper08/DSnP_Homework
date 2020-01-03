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
   auto it = _gateList.begin();
   IdList removedGateId;
   for (++it; it != _gateList.end(); ++it)
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
      
      // fanout remove
      GateList fanout = g->getFanoutGateList();
      for (size_t i = 0; i < fanout.size(); ++i)
      {
         GateList gFanin = fanout[i]->getFaninGateList();
         for (size_t j = 0; j < gFanin.size(); ++j)
            if (gFanin[j] == g) { fanout[i]->removeFanin(j); break; }
      }

      // fanin remove
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
}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/
