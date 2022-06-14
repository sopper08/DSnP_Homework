# README  

## Problems

*more detail, please see [Homework_5.pdf](./Homework_5.pdf).*

* Implement various ADTs including `doubly linked list`, `dynamic array` and `binary search tree`.

  * `make [d/a/b]`
    * `d`: `./adtTest.dlist` (doubly linked list)
    * `a`: `./adtTest.array` (dynamic array)
    * `b`: `./adtTest.bst` (binary search tree)

* `class AdtTest` contains an ADT (`AdtType<AdtTestObj> _container`) to store the objects of `class AdtTestObj`.

* `class AdtTestObj`

  * Its value is specified by
    * `ADTAdd -String`
    * `ADTAdd -Random`
  * `int _strLen` is to confine the maximum string length for `AdtTestObj::_str`.

* Command

  * There should be a command to reset class AdtTest. The AdtTestObj::\_strLen will be reset and the ADT in class AdtTest will be cleared.
  * There should be commands to add or delete objects to the ADT.
  * There should be a command to sort the class AdtTestObj objects stored in class DList and class Array. Note that you don’t need to maintain the order of the data for other commands. For class DList, please sort the data using the data structure itself (i.e. DList), DO NOT copy the data to other data structure (e.g. Array) just to lower the computational complexity. Ideally, the space complexity (i.e. extra memory required) for sorting should be O(1). Data in class BSTree by definition should always be sorted.

* There is also a data member bool \_isSorted for these two classes. You can
  then optionally use it to indicate whether the data is in a sorted state and thus
  take advantage of it to find a specific data whenever applicable. However,
  there is no command to report the value of this data member. Therefore, you
  2can design your own mechanism to maintain its value for your own
  optimization purpose (and of course, you can also choose NOT to use it).

* You should be able to find an element in the ADT, and print the elements of
  the ADT either in forward or backward order.

* All the ADTs should contain the following member functions

  * `iterator begin()`
    * Return the iterator pointing to the first element.
    * If ADT is empty
      * Return `end()`
    * Else (For BSTree)
      * Return the leftmost element.
  * `iterator end()`
    * return the `past-the-end` iterator.
    * DList
      * If is empty
        * `end() = begin() = iterator(_head) = the dummy iterator`
      * Else
        * end() is a dummy iterator whose DListNode has `iterator(_next) = begin()`, and `iterator(_prev) = the last element in the list`.
    * Array
      * If the Array is NOT yet initialized (i.e. \_capacity \=\= 0)
        * Both `begin()` and `end()` are 0.
      * Else
        * `end()` points to the next address of the last element. ,
    * BSTree
      * You can design it on your own. But make sure the `--` operator will bring the iterator back to the last element (if ADT is not empty).
  * `bool empty()`
    * check whether the ADT is empty.
  * `size_t size()`
    * Return the number of elements in the ADT.
  * `void pop_front()`
    * If the ADT is empty
      * no error will be issue
    * Else
      * Remove the first element in the ADT
      * For DList
        * The DListNode\* \_head will be updated and point to its next node.
      * For Array
        * The first element will be removed and, if \_size >\= 2, the last element will be `copied` to the first position.
        * The \_data pointer itself and \_capacity will NOT be changed.
      * For BSTree, its leftmost node (i.e. begin()) will be removed.

* `void pop_back()`

  * If the ADT is empty
    * no error will be issue
  * Else
    * Remove the last (rightmost for BSTree) element

* `bool erase(const T& x)`

  * If x does not exist in the ADT
    * return False
  * Else
    * If there are multiple existences of element x in the ADT
      * Remove the firstly encountered one (i.e. by traversing from `begin()`)
    * Else
      * Remove the element

* `bool erase(iterator pos)`

  * If x does not exist in the ADT
    * return False
  * Else
    * Remove the element in the pos of the ADT.

* `iterator find(const T& x)`

  * Check whether the element ‘x’ is in the ADT.
    * If yes
      * Return the iterator where ‘x’ is found
    * Else
      * Return `end()`.

* `void clear()`

  * empty the ADT
    * `DList`, `BSTree`
      * Delete all of their `DListNode` and `BSTreeNode`.
      * Do not delete the dummy `DListNode` and `BSTreeNode`.
    * `Array`
      * Reset its \_size to 0

* Constructor

  * `DList`
    * Allocate a dummy DListNode for \_head \= this dummy node. Its \_prev and \_next are pointing to itself.
  * `Array`
    * set \_data \= 0, \_size \= 0, and \_capacity \= 0.
    * In the later data insertions, the \_capacity will grow 0, 1, 2, 4, ... , 2^n ...
    * You should decide the initial value for the data member \_isSorted. You can even ignore it if you don’t use it.
  * `BSTree`
    * Since you should define the data members of the class BSTree on your own, you should also define its constructor by yourself.

* `class iterator` for each ADT

  * `*li`, `++li`, `li++`, `--li`, `li--`, `=`, `!=`, and `==`
  * Array::iterator\+(int) and \+\=(int)

* Supported Commands

  * `ADTReset <(size_t strLen)>`
    * Reset maximum string length for class AdtTestObj objects
    * Clean up the ADT in class AdtTest (i.e. by calling clear() of the ADT class).
  * `ADTAdd <-String (string str) | -Random (size_t repeats)>`
    * `ADTAdd -String (string str)`
      * For `Array` and `DList`
        * the specified string is added to the end of ADT.
      * For `BSTree`, the string should be inserted to the proper position of the sorted ADT.
    * `ADTAdd -Random (size_t repeats)`
      * Generate strings in all lower-case letters and with length equal to `AdtTestObj::_strLen`.

  * `ADTDelete <-All|-String (stirng str)|<<-Front|-Back|-Random (size_t repeats)>>`

    * `ADTDelete <-All|-String (stirng str)>`
      * `-All`: delete the entire ADT
      * `-String (stirng str)`: a specific string
        * If not found
          * `Error: "[str]" is not found!`
        * If there are multiple elements
          * delete the first encounter and leave the other(s) alone.
    * `ADTDelete <-Front|-Back|-Random (size_t repeats)>`
      * `-Front`: the first
      * `-Back`: the last
      * `-Random`

  * `ADTQuery <(stirng str)>`

    * If The length of `str>AdtTestObj::_strLen`
      * `Error: "[str]" exceeds string length limit!!`
    * Else
      * If not found
        * `"[str]" is not found!!`
      * Else
        * `"[str]" is found.`

  * `ADTSort`

    * Sort the objects in ascending order.
    * For BSTree, this is a dummy command.

  * `ADTPrint [-Reversed | (int n)]`

    * If no n
      * Print out the entire ADT in forward(if no `-Reversed`) order, or backward(if `-Rerversed`) order.
    * Else
      * If `n<0||n>size`
        * `Error: "[n]" is not a legal index!!`
      * Else
        * for linked list and array
          * it is the n th element in line (indexing starts from ‘0’)
        * for binary search tree
          * it is the n th smallest element in the ADT (n\=0 is the smallest).