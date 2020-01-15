/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir simulation functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <bitset>
#include "cirDef.h"
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Keep "CirMgr::randimSim()" and "CirMgr::fileSim()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/************************************************/
/*   Public member functions about Simulation   */
/************************************************/
void
CirMgr::randomSim()
{
}

void
CirMgr::fileSim(ifstream& patternFile)
{
   vector<string> lines;
   string         line;

   /**************************
    *       read file        *
    **************************/
   while (getline(patternFile, line)) lines.push_back(line);
   if (lines.empty()) return; // if no content in this file

   /**************************
    *  convert to P pattern  *
    **************************/
   ParallelPattern pPattern;
   if (!cvt2ParallelPattern(lines, pPattern, _piList.size())) return;

   /**************************
    *        simulate        *
    **************************/
   SimulateResult sR;
   simulate(pPattern, sR);

   /**************************
    *         output         *
    **************************/
   for (size_t i = 0; i < lines.size(); ++i)
   {
      *_simLog << lines[i] << " ";
      for (size_t j = 0; j < _poList.size(); ++j)
         *_simLog << sR[j][i];
      *_simLog << endl;
   }
   cout << lines.size() << " patterns simulated.";

   return;
}

/*************************************************/
/*   Private member functions about Simulation   */
/*************************************************/

void 
CirMgr::simulate(ParallelPattern& pPattern, SimulateResult& sR)
{
   ParallelPattern psR;

   for (size_t i = 0; i < pPattern.size(); ++i)
   {
      vector<size_t> tmp;
      for (auto it = _poList.begin(); it != _poList.end(); ++it)
          tmp.push_back((*it)->simulate(pPattern[i]));

      psR.push_back(tmp);
   }

   for (size_t i = 0; i < _poList.size(); ++i)
   {
      vector<bool> tmp;
      for (size_t j = 0; j < psR.size(); ++j)
      {
         bitset<64> r(psR[j][i]);
         for (int k = 63; k >= 0; --k)
            tmp.push_back(r[k]);
      }
      sR.push_back(tmp);
   }
}


bool
CirMgr::cvt2ParallelPattern(vector<string>& lines, ParallelPattern& p, unsigned piSize)
{
   unsigned lNo = 0, bit = sizeof(size_t)*8;

   vector<size_t> pp(piSize, 0);
   while (lNo < lines.size())
   {
     string line = lines[lNo++];
     --bit;
     
     if (line.size() != piSize) return false;
     
     for (size_t i = 0; i < line.size(); ++i)
     {
        bool b;
        if (!char2bool(line[i], b)) return false;
        pp[i] |= (((size_t)b) << bit);
     }

     if ((bit == 0) || (lNo == lines.size()))
     {
        p.push_back(pp);
        fill(pp.begin(), pp.end(), 0); bit = sizeof(size_t)*8;
     }
   }
   return true;
}

bool
CirMgr::char2bool(char s, bool& n)
{
   n = (s == '1');
   return true;
}
