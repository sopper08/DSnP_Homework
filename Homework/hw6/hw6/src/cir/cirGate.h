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
      _gateID(g), _lineNo(++l), _typeStr(t), _globalRef(false), _active(false), _fanoutGate(this), _fanoutGateInvPhase(false){ }
   virtual ~CirGate() {}

   // Basic access methods
   string getTypeStr() const { return _typeStr; }
   unsigned getGateID() const { return _gateID; }
   unsigned getLineNo() const { return _lineNo; }
   IdList getFaninIdList() const { return _faninID; }
   GateList getFaninGateList() const { return _faninGateList; }
   CirGate* getFanoutGate() const { return _fanoutGate; }
   vector<bool> getFaninGateInv() const { return _invPhase; }

   // Printing functions
   virtual void printGate() const = 0;
   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;

   // set fanin & fanout
   void setFanin(GateList& gL) { _faninGateList = gL; };
   void setFanout(CirGate*& g, bool i) { _fanoutGate = g; _fanoutGateInvPhase = i;}

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

   // void _faninTraversal(int, vector<pair<unsigned, CirGate*>>&);

protected:
   GateList _faninGateList;
   CirGate* _fanoutGate;
   IdList _faninID;
   IdList _fanoutID;
   vector<bool> _invPhase;
   bool _fanoutGateInvPhase;

};

class CirPiGate: public CirGate
{
public:
   CirPiGate(unsigned g, unsigned l, string s = "PI"): CirGate(g, l, s)
   {
      // cout << "Add new gate: " << getTypeStr() << endl;
      // cout << "_lineNo     : " << getLineNo() << endl;
      // cout << "_gateID     : " << getGateID() << endl;
   } 

   void printGate() const {}

private:

};

class CirPoGate: public CirGate
{
public:
   CirPoGate(unsigned g, unsigned l, unsigned inID, bool inv): CirGate(g, l, "PO")
   {
      _faninID.push_back(inID);
      _invPhase.push_back(inv);
      // cout << "Add new gate: " << getTypeStr() << endl;
      // cout << "_lineNo     : " << getLineNo() << endl;
      // cout << "_gateID     : " << getGateID() << endl;
      // cout << "_fanin0ID   : " << inID << endl;
      // cout << "_invPhase   : " << inv << endl;
   }

   void printGate() const {}

private:

};

class CirAigGate: public CirGate
{
public:
   CirAigGate(unsigned g, unsigned l, unsigned in0ID, unsigned in1ID, bool inv0, bool inv1):
      CirGate(g, l, "AIG")
   {
      _faninID.push_back(in0ID); _faninID.push_back(in1ID);
      _invPhase.push_back(inv0); _invPhase.push_back(inv1);
      // cout << "Add new gate: " << getTypeStr() << endl;
      // cout << "_lineNo     : " << getLineNo() << endl;
      // cout << "_gateID     : " << getGateID() << endl;
      // cout << "_fanin0ID   : " << in0ID << endl;
      // cout << "_fanin1ID   : " << in1ID << endl;
      // cout << "_inv0Phase  : " << inv0 << endl;
      // cout << "_inv1Phase  : " << inv1 << endl;
   }

   void printGate() const {}

private:

};

class CirUndefGate: public CirGate
{
public:
   CirUndefGate(unsigned g, unsigned l = 0):
      CirGate(g, l, "UNDEF") { }

   void printGate() const { }
};

#endif // CIR_GATE_H
