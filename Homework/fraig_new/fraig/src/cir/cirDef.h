/****************************************************************************
  FileName     [ cirDef.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic data or var for cir package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_DEF_H
#define CIR_DEF_H

#include <vector>
#include <map>
#include <unordered_map>
#include "myHashMap.h"

using namespace std;

// TODO: define your own typedef or enum

class CirGate;
class CirMgr;
class SatSolver;
class Fan;
class Fans;
class StrashFunction;

class Header
{
public:
   Header() {}
   Header(vector<unsigned> v):
      _M(v[0]), _I(v[1]), _L(v[2]), _O(v[3]), _A(v[4]) {}

   unsigned _M, _I, _L, _O, _A;
};

typedef vector<CirGate*>                               GateList;
typedef vector<Fan*>                                   FanList;
typedef vector<unsigned>                               IdList;
typedef map<unsigned, CirGate*>                        GateMap;
typedef map<unsigned, FanList>                         FanListMap;
typedef pair<CirGate*, bool>                           Pair_CirGateAndInv;
typedef unordered_map<Fans, CirGate*, StrashFunction>  Strash;
typedef vector<vector<size_t>>                         ParallelPattern;
typedef vector<vector<bool>>                           SimulateResult;


#endif // CIR_DEF_H
