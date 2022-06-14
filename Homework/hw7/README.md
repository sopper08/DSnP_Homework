# README

## Problem
*more detail, please see [Homework_7.pdf](./Homework_7.pdf).*

* Implement a task manager (using heap and hash).
* A task manager (a global variable `taskMgr` of `class TaskMgr`) to supervise various task nodes (`class TaskNode`).
* `class TaskNode`
  * Stores the information of the machine that executes the task
    * machine name *(should have distinct machine names)*
    * accumulated workload
* A hash (`class HashSet<TaskNode>`) to look up the task node by machine name
* a heap (`class MinHeap<TaskNode>`) to record and dynamically update the minimum task node (i.e. the one with the minimum accumulated load)
* Supported commands:
  * `TASKInit <(size_t numMachines)>`
    * `numMachines`
      * The approximated number of task machines (i.e. task nodes) that you expect to add later.
    * initialize the hash with the getHashSize(nMachines) (note: defined in “util/util.cpp”) number of buckets
    * reserve the memory for the heap (i.e. capacity = numMachines).
    * Others
      * If the global task manager “taskMgr” is initially NOT NULL, it will be deleted and reconstructed. A warning message “Warning: Deleting task manager... ” will be issued in such case.
  * `TASKNew <-Random (size_t numMachines)|-Name (string name) (size_t load)>`
    * `TASKNew <-Random (size_t numMachines)`
      * `numMachines` number of task nodes should be created
      * If a node with duplicated name is generated
        * Discarded and regenerated.
    * `TASKNew <-Name (string name) (size_t load)>`
      * If the machine name collides
          * Print an error message: `Error: Task node [name] already exists.`
       * Else
          * Insert new task nodes to the task manager
  * `TASKRemove <-Random (size_t nMachines)|-Name (string name)>`
    * `TASKRemove -Random [nMachines]`
      * if `nMachine > size of task nodes`
        * Remove all.
        * Print a warning massage
      * For each of the random removal, a random number i between 0 and s-1 (s is the number of total task nodes) is first generated and the i th element of the heap will be removed (by MinHeap::delData(i)).
  * `TASKQuery <(string name)|-HAsh|-HEap|-MINimum >`
    * `TASKQuery [name]`
      * If is not such node
        * print out an error message: `Query fails!`
      * Else
        * query the node with that name and print it.
    * `TASKQuery -HAsh`, `TASKQuery -HEap`, 
      * print all task nodes with the order as stored in the hash and heap,
    * `TASKQuery -MINimum`
      * print out the one with the minimum workload
  * `TASKAssign <(size_t load)> [-Repeat (size_t repeats)]`
    * Assign the workload to the task node with the minimum accumulated workload.
    * Note that every time the workload is assigned, the accumulated workload of the minimum task node will be incremented by “load” and the minimum task node (in the heap) will be updated accordingly. Therefore, it is likely that the repeated workload will be assigned to different task nodes.
