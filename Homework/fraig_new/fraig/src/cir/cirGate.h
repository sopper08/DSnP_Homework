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
#include <iomanip>
#include <algorithm>
#include "cirDef.h"
#include "sat.h"

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

class CirGate;

class Fan
{
public:
   Fan(CirGate* g, bool i): _gate(g), _inv(i) { }
   ~Fan() {}

   CirGate* getGate() const { return _gate; }
   bool     getInv()  const { return _inv;  }

private:
   CirGate* _gate;
   bool     _inv;
};

class Fans
{
public:
   Fans() { }
   ~Fans() { }

   // bool isEmptyFanin() const { return fanin.empty(); }

   FanList  getFaninList() const { return fanin; }
   FanList  getFanoutList() const { return fanout; }
   GateList getFaninGateList() const
   {
      GateList g;
      for (size_t i = 0; i < fanin.size(); ++i) g.push_back(fanin[i]->getGate());
      return g;
   }
   unsigned getFaninSize() const { return fanin.size(); }
   unsigned getFanoutSize() const { return fanout.size(); }

   bool removeFaninGate(CirGate*& g)
   {
      /*
      for (size_t i = 0; i < fanin.size(); ++i)
         if (fanin[i]->getGate() == g) { fanin[i] = fanin.back(); fanin.pop_back(); return true; }
   
      return false;
      */
      size_t i = 0;
      for (; i < fanin.size(); ++i)
         if (fanin[i]->getGate() == g) break;
      
      if (i == fanin.size()) return false;
      fanin.erase(fanin.begin()+i);
      return true;
   }

   bool removeFanoutGate(CirGate* g)
   {
      // for (size_t i = 0; i < fanout.size(); ++i)
      //    if (fanout[i]->getGate() == g) { fanout[i] = fanout.back(); fanout.pop_back(); return true; }
   
      // return false;
      size_t i = 0;
      for (; i < fanout.size(); ++i)
         if (fanout[i]->getGate() == g) break;
      
      if (i == fanout.size()) return false;
      fanout.erase(fanout.begin()+i); return true;
   }
   
   void setFanin(vector<Pair_CirGateAndInv>& gs)
   {
      assert(gs.size() < 3);
      for (auto it = gs.begin(); it != gs.end(); ++it)
         fanin.push_back(new Fan(it->first, it->second));
   }

   void addFanin(Fan* f) { fanin.push_back(f); }

   void addFanout(Fan* f) { fanout.push_back(f); }
   void addFanout(CirGate*& g, bool inv) { fanout.push_back(new Fan(g, inv)); }

   bool isSameFaninGate(FanList::const_iterator& it) const 
   {
      if ((fanin[0]->getGate())==(fanin[1]->getGate())) { it = fanin.begin(); return true; }
      else { it = fanin.end(); return false; }
   }

   FanList::iterator findFaninGate(CirGate*& g, bool returnAnother = false)
   {
      assert(fanin.size()==2);

      for (size_t i = 0; i < fanin.size(); ++i)
      {
         if ((fanin[i]->getGate())==g)
         {
            if (returnAnother) return fanin.begin()+(1-i);
            else return fanin.begin()+i;
         }
      }
      return fanin.end();
   }

   bool isSameFaninInv() const
   {
      assert(fanin.size()==2);
      return (fanin[0]->getInv()==fanin[1]->getInv());
   }

   bool replaceFanin(CirGate*& g, Fan*& f)
   {
      if (!removeFaninGate(g)) return false;
      addFanin(f); return true;
   }

   bool replaceFanout(CirGate*& g, Fan*& f)
   {
      if (!removeFanoutGate(g)) return false;
      addFanout(f); return true;
   }

   FanList::const_iterator faninEnd() const { return fanin.end(); }

private:
   FanList fanout;
   FanList fanin;
};

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
class CirGate
{
public:
   CirGate(unsigned gId, unsigned lNo, string typeN):
      _gateId(gId), _lineNo(lNo), _typeStr(typeN) { }

   virtual ~CirGate() {}

   bool operator== (const CirGate* g) { return (this->getGateId()==g->getGateId()); }

   // Basic access methods
   string  getTypeStr() const { return _typeStr; }
   unsigned getGateId() const { return _gateId; }
   unsigned getLineNo() const { return _lineNo; }
   bool isAig() const { return !(this->getTypeStr().compare("AIG")); }

   // Printing functions
   virtual void printGate() const {}
   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;

   /**************************
    *    Gate Information    *
    **************************/
   void setSymbol(const string& s) { _symbol = s; }

   /**************************
    *        For dfs         *
    **************************/
   void dfsTraversal(GateList& g)
   {
      GateList faninGateList = fanList.getFaninGateList();
      for (auto it = faninGateList.begin(); it != faninGateList.end(); ++it)
      {
         if (!(*it)->getTypeStr().compare("UNDEF")) continue;
         if (!(*it)->isGlobalRef())
         {
            (*it)->setToGlobalRef();
            (*it)->setActive();
            (*it)->dfsTraversal(g);
            (*it)->unsetActive();
         }
      }
      g.push_back(this);
   }
   bool isGlobalRef() const { return _globalRef;  }
   void setToGlobalRef()    { _globalRef = true;  }
   void unsetToGlobalRef()  { _globalRef = false; }
   bool isActive() const    { return _active;     }
   void setActive()         { _active = true;     }
   void unsetActive()       { _active = false;    }

   /**************************
    *          Fan           *
    **************************/
   Fans  fanList;
   
private:
   /**************************
    *        For dfs         *
    **************************/
   bool _globalRef, _active;

protected:
   /**************************
    *    Gate Information    *
    **************************/
   unsigned _gateId;
   unsigned _lineNo;
   string   _typeStr;
   string   _symbol;

};

class CirPiGate: public CirGate
{
public:
   CirPiGate(unsigned gId, unsigned lNo): CirGate(gId, ++lNo, "PI") {}
   ~CirPiGate() {}

   void printGate() const 
   { 
      cout << left << setw(3) << this->getTypeStr() << " " << this->getGateId();
      if (!_symbol.empty()) cout << " (" << _symbol << ")";
      cout << endl;
   }

private:

};

class CirPoGate: public CirGate
{
public:
   CirPoGate(unsigned gId, unsigned lNo, unsigned fin): 
      CirGate(gId, ++lNo, "PO"), oriFanin(fin) { }
   ~CirPoGate() {}

   void printGate() const 
   { 
      cout << left << setw(3) << this->getTypeStr() << " " << this->getGateId();
      FanList faninList = this->fanList.getFaninList();
      for (size_t i = 0; i < faninList.size(); ++i)
      {
         cout << " ";
         if (!(faninList[i]->getGate()->getTypeStr().compare("UNDEF"))) cout << "*"; // if undef fanin
         if (faninList[i]->getInv()) cout << "!"; // if inv = true
         cout << faninList[i]->getGate()->getGateId();
      }
      if (!_symbol.empty()) cout << " (" << _symbol << ")";
      cout << endl;
   }
   
   unsigned oriFanin;
};

class CirAigGate: public CirGate
{
public:
   CirAigGate(unsigned gId, unsigned lNo, unsigned fin0, unsigned fin1):
      CirGate(gId, ++lNo, "AIG"), oriFanin0(fin0), oriFanin1(fin1) {}
   ~CirAigGate() {}
   
   void printGate() const
   { 
      cout << left << setw(3) << this->getTypeStr() << " " << this->getGateId();
      FanList faninList = this->fanList.getFaninList();
      for (size_t i = 0; i < faninList.size(); ++i)
      {
         cout << " ";
         if (!(faninList[i]->getGate()->getTypeStr().compare("UNDEF"))) cout << "*"; // if undef fanin
         if (faninList[i]->getInv()) cout << "!"; // if inv = true
         cout << faninList[i]->getGate()->getGateId();
      }
      cout << endl;
   }
   
   bool isAig() { return true; }
   
   unsigned oriFanin0, oriFanin1;
};

class CirConstGate: public CirGate
{
public:
   CirConstGate(unsigned gId = 0, unsigned lNo = 0): CirGate(gId, lNo, "CONST") { }
   ~CirConstGate() { }
   
   void printGate() const { cout << left << setw(3) << this->getTypeStr() << this->getGateId() << endl; }

private:

};

class CirUndefGate: public CirGate
{
public:
   CirUndefGate(unsigned g, unsigned l = 0):
      CirGate(g, l, "UNDEF") { }
   ~CirUndefGate() { }

   void printGate() const { cout << left << setw(3) << this->getTypeStr() << " " << this->getGateId(); }
};




#endif // CIR_GATE_H
