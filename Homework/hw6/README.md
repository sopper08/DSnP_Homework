# README  

## Problem
*more detail, please see [Homework_6.pdf](./Homework_6.pdf).*
* Implement a special circuit representation, called AIG(And-Inverter Graph), from a circuit description file.
* Parsing the circuit description file in the AIGER format.
* Detect the following types of errors in the file. (start from `1`)
  * `Cannot open design “c17.aag”!!`
  * `[ERROR] Line 1: Illegal identifier "aagg"!!`
  * `[ERROR] Line 1, Col 5: Missing number of variables!!`
  * `[ERROR] Line 1: Number of variables is too small (3)!!`
  * `[ERROR] Line 1, Col 5: Extra space character is detected!!`
  * `[ERROR] Line 6, Col 2: Missing space character!!`
  * `[ERROR] Line 9, Col 1: Illegal white space char(9) is detected!!`
  * `[ERROR] Line 2: Missing PI definition!!`
  * `[ERROR] Line 8: Missing AIG definition!!`
  * `[ERROR] Line 3, Col 1: Missing PI literal ID!!`
  * `[ERROR] Line 4, Col 1: Missing PO literal ID!!`
  * `[ERROR] Line 9: Missing "symbolic name"!!`
  * `[ERROR] Line 1, Col 14: A new line is expected here!!`
  * `[ERROR] Line 3: Literal "2" is redefined, previously defined as PI in line 1!!`
  * `[ERROR] Line 7: Literal "6" is redefined, previously defined as AIG in line 5!!`
  * `[ERROR] Line 3, Col 3: Literal "16" exceeds maximum valid ID!!`
  * `[ERROR] Line 3, Col 1: Cannot redefine constant (1)!!`
  * `[ERROR] Line 9: Illegal symbol index(a)!!`
  * `[ERROR] Line 9, Col 1: Illegal symbol type ( )!!`
  * `[ERROR] Line 9, Col 1: Illegal symbol type (a)!!`
  * `[ERROR] Line 9, Col 5: Symbolic name contains un-printable char(27)!!`
  * `[ERROR] Line 10: Symbolic name for "i0" is redefined!!`
  * `[ERROR] Line 9: PI index is too big (3)!!`
* Use `static bool parseError(CirParseError err)` in `cirMgr.cpp` to print error.
* Assume that the netlist is a directed acyclic graph (DAG) and don’t need to check for the loops.
* Gate situations
  * A gate that cannot reach any PO -> `defined but not used`
  * A gate that be reached from PI -> `a gate with floating fanins`
  * More specifically, we call the floating fanins in (b) “referenced but not defined”, and designate “UNDEF” gate type for them for netlist reporting (See examples in CIRGate command). cannot be reached and PO from any PI
* You should be able to report the circuit in various aspects
  * circuit statistics/summary
  * circuit status
  * circuit netlist in depth-first-search (DFS) order which should be constructed by recursive calls from the primary outputs in post-order traversal.
    * Floating gates of the cases (a), and (c) above should not be included in the DFS list (Note: cases (b) and (d) should be included) since they cannot be reached from POs.
    * Constant gate (i.e. CONST0) should be included if it is visited during the DFS traversal.
    * You should also provide commands to report the interconnections (i.e. lists of fanins) of a gate.
  * lists of primary inputs/outputs
  * list of floating gates
  * The order of POs, as well as the order of fanins in a gate, should follow the orders as defined in the original AIGER file.
* Each gate should have a unique gate ID, which is equivalent to the variable ID as derived in the AIGER file.
* The IDs for PIs and AIG gates are the values of the corresponding literal IDs divided by 2.
* Supported Commands
  * `CIRRead <(string fileName)> [-Replace]`
    * If the circuit has been read
      * If `-Replace` is not issued
        * `Error: circuit already exists!!`
      * Else
        * delete the original circuit and discard all the circuit related data
        * create a new one
        * print out the message `Note: original circuit is replaced...`
        * In such case, cirMgr should be reset to NULL (0).
    * Else
      * Read the circuit from the file `fileName` and store it in the global variable cirMgr.
  * `CIRPrint [-Summary|-Netlist|-PI|-PO|-FLoating]`
    * If the circuit hasn’t been constructed
      * Print out an error message `Error: circuit has not been read!!`
    * Else
      * `-Summary`: print out the circuit statistics
        * the statistics of gates should be in the order: { PI, PO, AIG }, no other gate types.
      * `-Netlist`: ist all the gates in the topological (depth-first-search, DFS) order
      * `-PI`: the lists of primary inputs (PIs)
      * `-PO`: the lists of primary outputs (POs)
      * `-FLoating`: the lists of floating gates
  * `CIRGate <<(int gateId)> [<-FANIn | -FANOut><(int level)>]>`
  * `CIRWrite [-Output (string aagFile)]`