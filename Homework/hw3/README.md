# README  

## Problem
*more detail, please see [Homework_3.pdf](./Homework_3.pdf).*

* Create DB by reading in data from a .json (JSON) file.
* Sort the JSON elements by either `key` or `value` in ascending order.
* Compute the summation and average of the JSON elements by their `values`.
* Supported Commands
  * `DBAPpend <(string key)><(int value)>`
    * Append an JSON element (i.e. key-value pair) to the end of the DB.
    * Key
      * Composed of English letters (i.e. a-z, A-Z), digits (i.e. 0-9), and underline symbol
      * The first character of the `key` must be an English letter or underline symbol.
    * Value
      * Must be a valid integer.
  * `DBAVerage`
    * If DB is empty
      * Report NAN.
    * Else
      * Print out the average of the values in the DB. (`fixed`, `setprecision(2)`)
  * `DBCount`
    * Print out the number of JSON element(s) in the DB.
  * `DBMAx`
    * If the DB is empty
      * Report NAN.
    * Else
      * Print out the maximum JSON element.
  * `DBMIn`
    * If the DB is empty
      * Report NAN.
    * Else
      * Print out the minimum JSON element.
  * `DBPrint [(string key)]`
    * if no key
      * Print out the entire JSON object.
    * Else
      * Print out the JSON element with the the `key`.
  * `DBRead <(string jsonFile)> [-Replace]`
    * If file `jsonFile` doesn’t exist
      * Error
    * Else
      * If the DB already exists
        * If the option `-Replace` is not specified
          * Error
        * Else
          * Replace the DB content with the data in the `jsonFile`.
      * Else
        * Read the data from `jsonFile` to the database (DB).
  * `DBSOrt <-Key | -Value>`
    * Sort the JSON elements with key (-Key) or value (-Value) in ascending order.
  * `DBSUm`
    * If the DB is empty
      * Report NAN.
    * Else
      * Print out the summation of the values of the JSON elements.
  * `DOfile <(string filename)>`
    * Execute the commands in the dofile, and then, go back to the command prompt.
  * `HELp [(string cmd)]`
    * If command is specified
      * Print out its usage.
    * Else
      * Print out the list of all commands with simple descriptions.
  * `HIStory [(int nPrint)]`
    * Print command history.
    * If `nPrint` is specified
      * Print the last `nPrint` command history.
    * Else
      * Print all the histories.
  * `Quit [-Force]`
    * Quit the execution.