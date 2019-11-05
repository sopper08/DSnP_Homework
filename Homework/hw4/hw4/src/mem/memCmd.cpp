/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
   // TODO
   vector<string> tokens;
   if (!lexOptions(option, tokens)) { return CMD_EXEC_ERROR; }
   if (tokens.empty()) { return CmdExec::errorOption(CMD_OPT_MISSING, ""); }

   /* Check the token & Assign them to 
    *   1. numObjects
    *   2. arraySize
    */
   bool doArray = false;
   bool isArray = false;
   string tmp;
   int numObjects = 0;
   int arraySize = 0;
   for (size_t i = 0, n = tokens.size(); i < n; ++i)
   {
      if (myStrNCmp("-Array", tokens[i], 2) == 0)
      {
         if (doArray) { return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[i]); }
         doArray = isArray = true;
         tmp = tokens[i];
      }
      else
      {
         if (doArray) 
         {
            if (arraySize != 0)
               return CmdExec::errorOption(CMD_OPT_EXTRA, tokens[i]);
            if (!myStr2Int(tokens[i], arraySize)) 
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[i]);
            if (arraySize <= 0)
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[i]);
            doArray = false;
         }
         else
         {
            if (numObjects != 0) 
               return CmdExec::errorOption(CMD_OPT_EXTRA, tokens[i]);
            if (!myStr2Int(tokens[i], numObjects)) 
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[i]);
            if (numObjects <= 0)
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[i]);
         }
      }
   }
   if (arraySize == 0 && isArray) { return CmdExec::errorOption(CMD_OPT_MISSING, tmp); }
   if (numObjects == 0) { return CmdExec::errorOption(CMD_OPT_MISSING, tmp); }
   
   /* Allocate memory test object */
   try
   {
      if (arraySize==0) { mtest.newObjs(numObjects); }
      else { mtest.newArrs(numObjects, arraySize); }
   }
   catch(bad_alloc& ba)
   {
      return CMD_EXEC_NOP;
   }
   
   // Use try-catch to catch the bad_alloc exception
   return CMD_EXEC_DONE;
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
   // TODO
   vector<string> tokens;
   bool isArray = false, isIndex = false, isRandom = false;
   bool doIndex = false, doRandom = false;
   int objId = -1, numRandId = -1;
   string tmp;

   if (!lexOptions(option, tokens)) { return CMD_EXEC_ERROR; }
   if (tokens.empty()) { return CmdExec::errorOption(CMD_OPT_MISSING, ""); }

   for (size_t i = 0, n = tokens.size(); i < n; ++i)
   {
      if (doIndex)
      {
         if (objId != -1) 
            return CmdExec::errorOption(CMD_OPT_EXTRA, tokens[i]);
         if (!myStr2Int(tokens[i], objId)) 
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[i]);
         if (objId < 0)
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[i]);
         doIndex = false;
      }
      else if (doRandom)
      {
         if (numRandId != -1) 
            return CmdExec::errorOption(CMD_OPT_EXTRA, tokens[i]);
         if (!myStr2Int(tokens[i], numRandId)) 
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[i]);
         if (numRandId < 0)
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[i]);
         doRandom = false;
      }
      else if (myStrNCmp("-Array", tokens[i], 2) == 0)
      {
         if (isArray) { return CmdExec::errorOption(CMD_OPT_EXTRA, tokens[i]); }
         isArray = true;
      }
      else if (myStrNCmp("-Index", tokens[i], 2) == 0)
      {
         if (isIndex || isRandom) { return CmdExec::errorOption(CMD_OPT_EXTRA, tokens[i]); }
         isIndex = doIndex = true;
         tmp = tokens[i];
      }
      else if (myStrNCmp("-Random", tokens[i], 2) == 0)
      {
         if (isIndex || isRandom) { return CmdExec::errorOption(CMD_OPT_EXTRA, tokens[i]); }
         isRandom = doRandom = true;
         tmp = tokens[i];
      }
      else
      {
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[i]);
      }
      
   }
   // cout << "id     : " << objId << " / " << numRandId << endl;
   // cout << "isArray: " << isArray << endl;
   // cout << "isIndex / isRandom: " << isIndex << " / " << isRandom << endl;
   int size;

   if (((isIndex)  && (isRandom))   && (numRandId != -1)) { return CmdExec::errorOption(CMD_OPT_MISSING, tmp); }
   if (((isRandom) && (isIndex))    && (objId     != -1)) { return CmdExec::errorOption(CMD_OPT_MISSING, tmp); }
   if ((objId == -1) && (numRandId == -1)) { return CmdExec::errorOption(CMD_OPT_MISSING, ""); }
   if (isArray)
   { 
      size = mtest.getArrListSize();

      if (isIndex) { 
         if (objId >= size)
         {
            cerr << "Size of array list (" << size << ") is <= " << objId << "!!" << endl;
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, to_string(objId));
         }
         mtest.deleteArr(objId); 
      }
      if (isRandom)
      {
         if (size == 0)
         {
            cerr << "Size of array list is 0!!" << endl;
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, tmp);
         }
         for (int i = 0; i < numRandId; ++i) { mtest.deleteArr(rnGen(size)); }
      }
   }
   else 
   {  
      size = mtest.getObjListSize();

      if (isIndex) 
      {
         if (objId >= size)
         {
            cerr << "Size of object list (" << size << ") is <= " << objId << "!!" << endl;
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, to_string(objId));
         }
         mtest.deleteObj(objId);
      }
      if (isRandom)
      {
         if (size == 0)
         {
            cerr << "Size of object list is 0!!" << endl;
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, tmp);
         }
         for (int i = 0; i < numRandId; ++i) { mtest.deleteObj(rnGen(size)); }
      }
   }

   return CMD_EXEC_DONE;
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


