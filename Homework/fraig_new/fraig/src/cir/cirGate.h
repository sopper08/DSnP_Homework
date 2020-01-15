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

   bool operator == (const Fan& f) const
   {
      if (_inv != f.getInv()) return false;
      if (_gate != f.getGate()) return false;
      return true;
   }


   CirGate* getGate() const { return _gate; }
   bool     getInv()  const { return _inv;  }
   unsigned getGateId() const;

private:
   CirGate* _gate;
   bool     _inv;
};

class Fans
{
public:
   Fans() { }
   ~Fans() { }

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

   FanList::iterator findFaninGate(CirGate*& g)
   {
      for (size_t i = 0; i < fanin.size(); ++i)
         if ((fanin[i]->getGate())==g) { return fanin.begin()+i; }

      return fanin.end();
   }

   FanList::iterator findFaninGate(CirGate*& g, bool returnAnother)
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


   FanList::iterator findFanoutGate(CirGate*& g)
   {
      for (size_t i = 0; i < fanout.size(); ++i)
      {
         if ((fanout[i]->getGate())==g)
         {
            return fanout.begin()+i;
         }
      }
      return fanout.end();
   }

   bool isSameFaninInv() const
   {
      assert(fanin.size()==2);
      return (fanin[0]->getInv()==fanin[1]->getInv());
   }

   bool replaceFanin(CirGate*& g, Fan*& f)
   {
      FanList::iterator it = findFaninGate(g);
      if (it == this->faninEnd()) return false;
      (*it) = f; return true;
   }

   bool replaceFanout(CirGate*& g, Fan*& f)
   {
      FanList::iterator it = findFanoutGate(g);
      if (it == this->fanoutEnd()) return false;
      (*it) = f; return true;
   }

   bool replaceFanin(CirGate*& g, CirGate* ng)
   {
      FanList::iterator it = findFaninGate(g);
      if (it == this->faninEnd()) return false;
      Fan *f = new Fan(ng, (*it)->getInv());
      (*it) = f; return true;
   }

   bool replaceFanout(CirGate*& g, CirGate* ng)
   {
      FanList::iterator it = findFanoutGate(g);
      if (it == this->fanoutEnd()) return false;
      Fan *f = new Fan(ng, (*it)->getInv());
      (*it) = f; return true;
   }

   FanList::const_iterator faninEnd() const { return fanin.end(); }

   FanList::const_iterator fanoutEnd() const { return fanout.end(); }

   bool operator== (const Fans& fs) const
   {
      FanList fsFanin = fs.getFaninList();
      if (fanin.size() != fsFanin.size()) return false;
      if (fanin.size() == 0) return false;
      if (fanin.size() == 1)
      {
         if (fanin[0]->getInv() != fsFanin[0]->getInv()) return false;
         if (fanin[0]->getGateId() != fsFanin[0]->getGateId()) return false;
      }
      if (fanin.size() == 2)
      {
         if (fanin[0]->getGateId() == fsFanin[0]->getGateId())
         {
            if (fanin[0]->getInv()    != fsFanin[0]->getInv())    return false;
            if (fanin[1]->getGateId() != fsFanin[1]->getGateId()) return false;
            if (fanin[1]->getInv()    != fsFanin[1]->getInv()   ) return false;
         }
         else if (fanin[0]->getGateId() == fsFanin[1]->getGateId())
         {
            if (fanin[0]->getInv()    != fsFanin[1]->getInv())    return false;
            if (fanin[1]->getGateId() != fsFanin[0]->getGateId()) return false;
            if (fanin[1]->getInv()    != fsFanin[0]->getInv()   ) return false;
         }
         else { return false; }
      }
      
      return true;
   }

private:
   FanList fanout;
   FanList fanin;
};

//------------------------------------------------------------------------
//   Define classes
//-----------------------------------------------------------------------
class CirGate
{
public:
   CirGate(unsigned gId, unsigned lNo, string typeN):
      _gateId(gId), _lineNo(lNo), _typeStr(typeN) { }

   virtual ~CirGate() {}

   size_t operator () () const { return _gateId; }
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
    *         strash         *
    **************************/
   // mg -> this
   // need be changed: 
   //   1. fanout of this (add fanout of mg)
   //   2. fanin of fanout of mg (replace into this)
   //   3. fanout of fanin of mg (replace tnto this)
   void merge(CirGate* mg)
   {
      FanList mgFanout = mg->fanList.getFanoutList();
      FanList mgFanin  = mg->fanList.getFaninList();
      
      for (auto it = mgFanout.begin(); it != mgFanout.end(); ++it)
      {  
         // it is fanout of mg
         FanList itFanin = (*it)->getGate()->fanList.getFaninList();
         this->fanList.addFanout((*it));
         for (auto itt = itFanin.begin(); itt != itFanin.end(); ++itt)
         {
            // itt is fanin of fanout of mg
            (*it)->getGate()->fanList.replaceFanin(mg, this);
         }
      }
      for (auto it = mgFanin.begin(); it != mgFanin.end(); ++it)
      {
         // it is fanin of mg
         FanList itFanout = (*it)->getGate()->fanList.getFanoutList();
         for (auto itt = itFanout.begin(); itt != itFanout.end(); ++itt)
         {
            // itt is fanout of fanin of mg
            (*it)->getGate()->fanList.replaceFanout(mg, this);
         }
      }
   }

   /**************************
    *         simulate       *
    **************************/
   virtual size_t simulate(vector<size_t>& signals) = 0;
   size_t getSignal() const { return _signal; }
   size_t setSignal(size_t s) { _signal = s; }

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
   size_t   _signal;

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

   size_t simulate(vector<size_t>&);

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

   size_t simulate(vector<size_t>&);
   
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

   size_t simulate(vector<size_t>&);
   
   bool isAig() { return true; }
   
   unsigned oriFanin0, oriFanin1;
};

class CirConstGate: public CirGate
{
public:
   CirConstGate(unsigned gId = 0, unsigned lNo = 0): CirGate(gId, lNo, "CONST") { }
   ~CirConstGate() { }
   
   void printGate() const { cout << left << setw(3) << this->getTypeStr() << this->getGateId() << endl; }
   size_t simulate(vector<size_t>&);

private:

};

class CirUndefGate: public CirGate
{
public:
   CirUndefGate(unsigned g, unsigned l = 0):
      CirGate(g, l, "UNDEF") { }
   ~CirUndefGate() { }

   void printGate() const { cout << left << setw(3) << this->getTypeStr() << " " << this->getGateId(); }
   size_t simulate(vector<size_t>&);

};

class StrashFunction
{
public:
   static bool compareFan(Fan *f1, Fan *f2) { return (f1->getGateId() < f2->getGateId()); }
   size_t operator() (const Fans& fs) const
   {
      FanList fanin = fs.getFaninList();
      sort(fanin.begin(), fanin.end(), compareFan);
      
      size_t r = 0;
      for (size_t i = 0; i < fanin.size(); ++i)
         r += (fanin[i]->getGateId() << (6*i));

      return r;
   }
};




#endif // CIR_GATE_H
