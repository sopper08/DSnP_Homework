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
      _gateList.insert({0, const0});
   }
   ~CirMgr() {} 

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const 
   {
      GateMap::const_iterator pos =  _gateList.find(gid);
      if (pos == _gateList.end()) return 0;
      return (pos->second);
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

   CirGate *const0;

private:
   /**************************
    *       read file        *
    **************************/
   bool readHeader(const string&);
   bool readInput(const string&, const int&);
   bool readOutput(const string&, const int&);
   bool readAig(const string&, const int&);
   bool readSymbol(string&);

   /**************************
    *        connect         *
    **************************/
   bool connect();
   bool rconnect();

   /**************************
    *         gate           *
    **************************/
   bool findGate(GateMap&, unsigned, GateMap::iterator&, bool addIt = true);

   /**************************
    *      floating gate     *
    **************************/
   void resetFloatingGates();
   bool existUNDEFGate(GateList&);

   /**************************
    *           dfs          *
    **************************/
   void resetDFSList();
   void resetDFSParameters();

   /**************************
    *           opt          *
    **************************/
   void optimize(CirGate*);
   void swapFanoutList(CirGate*& g, Fan* newFan);

   /**************************
    *       simulation       *
    **************************/
   void simulate(ParallelPattern&, SimulateResult&);
   bool cvt2ParallelPattern(vector<string>&, ParallelPattern&, unsigned);
   bool char2bool(char, bool&);

   Header             *_header;
   ofstream           *_simLog;

   GateMap            _gateList;
   GateList           _piList;
   GateList           _poList;
   GateList           _aigList;
   IdList             _floatingFaninIdList;
   IdList             _defButNotUsedIdList;

   GateList           _dfsList;


};

#endif // CIR_MGR_H
