# DSnP-Homework  
*DSnP means Data Structure and Programming*

## hw1  
*more detail, please see [hw1/README](Homework/hw1/README.md).*
* Implement a simple JSON reader CLI.
* Supported commands:
  * `ADD [key] [value]`: adds a new element
  * `SUM`: compute the summation
  * `AVE`: compute the average
  * `MAX`: compute the maximum
  * `MIN`: compute the minimum
  * `PRINT`: prints the entire JSON object
  * `EXIT`: exits the program

![](docs/hw1_demo.gif)

## hw2  
*more detail, please see [hw2/README](Homework/hw2/README.md).*
* Implement a CLI with the command history.
* Supported standard input:
  * up/down arrows, PgUp, and PgDn
    * Traverse the command history.
  * left/right arrows, Ctrl-a, Ctrl-e, Home and End
    * Move the cursor.
  * back space, delete
  * Tab

## hw3  
*more detail, please see [hw3/README](Homework/hw3/README.md).*
* Implement a simple DB CLI.
* Supported commands:
  * `DBAPpend <(string key)><(int value)>` and `DBRead <(string jsonFile)> [-Replace]`
  * `DBCount`, `DBAVerage`, `DBMAx`, `DBMIn` and `DBSUm`
  * `DBPrint [(string key)]`
  * `DBSOrt <-Key | -Value>`
  * `DOfile <(string filename)>`
  * `HELp [(string cmd)]`
  * `HIStory [(int nPrint)]`
  * `Quit [-Force]`

## hw4  
*more detail, please see [hw4/README](Homework/hw4/README.md).*


## hw5  
*more detail, please see [hw5/README](Homework/hw5/README.md).*
* Impletment abstract data types (ADTs) for sorting input data.
  * doubly linked list
  * dynamic array
  * binary search tree
* Supported commands:
  * `ADTReset <(size_t strLen)>`
  * `ADTAdd <-String (string str)|-Random (size_t repeats)>`
  * `ADTDelete <-All|-String (stirng str)|<<-Front|-Back|-Random (size_t repeats)>>`
  * `ADTQuery <(stirng str)>`
  * `ADTPrint [-Reversed | (int n)]`

## hw6  
*more detail, please see [hw6/README](Homework/hw6/README.md).*
* Implement a AIG(And-Inverter Graph) reader.
  * Parsing the circuit description file in the AIGER format.
  * Detect serveral types of errors in AIG file.
* Supported commands:
  * `CIRRead <(string fileName)> [-Replace]`
  * `CIRPrint [-Summary|-Netlist|-PI|-PO|-FLoating]`
    * `-Summary`: Print out the circuit statistics.
    * `-Netlist`: List all the gates in the topological (depth-first-search, DFS) order.
  * `CIRGate <<(int gateId)> [<-FANIn|-FANOut><(int level)>]>`
  * `CIRWrite [-Output (string aagFile)]`

## hw7  
*more detail, please see [hw7/README](Homework/hw7/README.md).*
* Implement a task manager (using heap and hash).
* Supported commands:
  * `TASKInit <(size_t numMachines)>`
  * `TASKNew <-Random (size_t numMachines)|-Name (string name) (size_t load)>`
  * `TASKRemove <-Random (size_t nMachines)|-Name (string name)>`
  * `TASKQuery <(string name)|-HAsh|-HEap|-MINimum >`
  * `TASKAssign <(size_t load)> [-Repeat (size_t repeats)]`