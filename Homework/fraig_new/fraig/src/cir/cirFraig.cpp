/****************************************************************************
  FileName     [ cirFraig.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir FRAIG functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "sat.h"
#include "myHashMap.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::strash()" and "CirMgr::fraig()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/*******************************************/
/*   Public member functions about fraig   */
/*******************************************/
// _floatList may be changed.
// _unusedList and _undefList won't be changed
void
CirMgr::strash()
{
   Strash strash;
   IdList removedGateId;

   for (auto it = _dfsList.begin(); it != _dfsList.end(); ++it)
   {
     if (!(*it)->isAig()) continue;
     auto pos = strash.find((*it)->fanList);
     if (pos != strash.end())
     {
        cout << "Strashing: ";
        cout << (*pos).second->getGateId();
        cout << " merging ";
        cout << (*it)->getGateId();
        cout << "..." << endl;

        removedGateId.push_back((*it)->getGateId());
        (*pos).second->merge((*it));
        _aigList.erase(remove(_aigList.begin(), _aigList.end(), *it), _aigList.end());
     }
     else { strash.insert({(*it)->fanList, (*it)}); }
   }
   for (size_t i = 0; i < removedGateId.size(); ++i)
      _gateList.erase(removedGateId[i]);
   
   resetDFSList();
   resetFloatingGates();
}

void
CirMgr::fraig()
{
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/
