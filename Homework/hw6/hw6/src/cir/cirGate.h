/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"

using namespace std;

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{
public:
   CirGate() {}
   CirGate(unsigned g, int l, string t):
      _gateID(g), _lineNo(l), _typeStr(t), _globalRef(false), _active(false) { }
   virtual ~CirGate() {}

   // Basic access methods
   string getTypeStr() const { return _typeStr; }
   unsigned getGateID() const { return _gateID; }
   unsigned getLineNo() const { return _lineNo; }
   string getSymbol() const { return _symbol; }
   // fanin
   IdList getFaninIdList() const { return _faninID; }
   GateList getFaninGateList() const { return _faninGateList; }
   vector<bool> getFaninGateInv() const { return _faninGateInvPhaseList; }
   // fanout
   GateList getFanoutGateList() const { return _fanoutGateList; }

   // Printing functions
   virtual void printGate() const = 0;
   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;

   // set fanin & fanout
   void setFanin(GateList& gL) { _faninGateList = gL; };
   void setFanout(CirGate*& g, bool i) { _fanoutGateList.push_back(g); _fanoutGateInvPhaseList.push_back(i); }

   // set symbol
   void setSymbol(string& s) { _symbol = s; }

   // dfs
   void dfsTraversal(GateList&);
   bool isGlobalRef() const { return _globalRef; }
   void setToGlobalRef() { _globalRef = true; }
   bool isActive() const { return _active; }
   void setActive() { _active = true; }
   void unsetActive() { _active = false; }

private:
   unsigned _gateID;
   unsigned _lineNo;
   string _typeStr;
   // for dfs
   bool _globalRef;
   bool _active;

protected:
   GateList _faninGateList;
   GateList _fanoutGateList;
   IdList _faninID;
   IdList _fanoutID;
   vector<bool> _faninGateInvPhaseList;
   vector<bool> _fanoutGateInvPhaseList;
   string _symbol;

};

class CirConstGate: public CirGate
{
public:
   CirConstGate(unsigned g = 0, unsigned l = 0): CirGate(g, l, "CONST") { }
   
   void printGate() const { cout << left << setw(3) << getTypeStr() << getGateID() << endl; }

private:

};

class CirPiGate: public CirGate
{
public:
   CirPiGate(unsigned g, unsigned l): CirGate(g, ++l, "PI") { }

   void printGate() const 
   { 
      cout << left << setw(3) << getTypeStr() << " " << getGateID();
      if (!_symbol.empty()) cout << " (" << _symbol << ")";
      cout << endl;
   }

private:

};

class CirPoGate: public CirGate
{
public:
   CirPoGate(unsigned g, unsigned l, unsigned inID, bool inv): CirGate(g, ++l, "PO")
   {
      _faninID.push_back(inID);
      _faninGateInvPhaseList.push_back(inv);
   }

   void printGate() const 
   { 
      cout << left << setw(3) << getTypeStr() << " " << getGateID();
      int invIdx = 0;
      for (auto it = _faninGateList.begin(); it != _faninGateList.end(); ++it)
      {
         cout << " ";
         if (!(*it)->getTypeStr().compare("UNDEF")) cout << "*";
         if (_faninGateInvPhaseList.at(invIdx)) cout << "!";
         cout << (*it)->getGateID();
         ++invIdx;
      }
      if (!_symbol.empty()) cout << " (" << _symbol << ")";
      cout << endl;
   }

private:

};

class CirAigGate: public CirGate
{
public:
   CirAigGate(unsigned g, unsigned l, unsigned in0ID, unsigned in1ID, bool inv0, bool inv1):
      CirGate(g, ++l, "AIG")
   {
      _faninID.push_back(in0ID); _faninID.push_back(in1ID);
      _faninGateInvPhaseList.push_back(inv0); _faninGateInvPhaseList.push_back(inv1);
   }

   void printGate() const
   { 
      cout << left << setw(3) << getTypeStr() << " " << getGateID();
      int invIdx = 0;
      for (auto it = _faninGateList.begin(); it != _faninGateList.end(); ++it)
      {
         cout << " ";
         if (!(*it)->getTypeStr().compare("UNDEF")) cout << "*";
         if (_faninGateInvPhaseList.at(invIdx)) cout << "!";
         cout << (*it)->getGateID();
         ++invIdx;
      }
      cout << endl;
   }

private:

};

class CirUndefGate: public CirGate
{
public:
   CirUndefGate(unsigned g, unsigned l = 0):
      CirGate(g, l, "UNDEF") { }

   void printGate() const { cout << left << setw(3) << getTypeStr() << " " << getGateID(); }
};



#endif // CIR_GATE_H
