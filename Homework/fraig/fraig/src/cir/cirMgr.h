/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

#include "cirDef.h"
#include "cirGate.h"

extern CirMgr *cirMgr;

class CirMgr
{
public:
   CirMgr()
   {
      const0 = new CirConstGate();
      _gateList[0] = const0;
   }
   ~CirMgr() 
   {
      reset();
   }


   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const 
   {
      map<unsigned, CirGate*>::const_iterator pos = _gateList.find(gid);
      if (pos == _gateList.end()) return 0;
      else return pos->second; 
   }

   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit optimization
   void sweep();
   void optimize();

   // Member functions about simulation
   void randomSim();
   void fileSim(ifstream&);
   void setSimLog(ofstream *logFile) { _simLog = logFile; }

   // Member functions about fraig
   void strash();
   void printFEC() const;
   void fraig();

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void printFECPairs() const;
   void writeAag(ostream&) const;
   void writeGate(ostream&, CirGate*) const;
   CirGate* const0;

private:
   bool readHeader(string&);
   bool readInput(string&, int);
   bool readOutput(string&, int);
   bool readAig(string&, int);
   bool readSymbol(string&);
   bool connect();
   bool rconnect();
   bool genDFSList();
   bool checkFloatingAndNotUsedGates();
   void reset();

   bool lexOptions(const string&, vector<int>&, size_t nOpts = 0) const;

   vector<int> _header;
   GateList    _piList;
   GateList    _poList;
   GateList    _aigList;
   GateList    _dfsList;
   GateSet     _dfsSet;  // for searching gate in _dfsList
   IdList      _floatingGates; 
   IdList      _notUsedGates;
   GateMap     _gateList;
   ofstream   *_simLog;

};

#endif // CIR_MGR_H
