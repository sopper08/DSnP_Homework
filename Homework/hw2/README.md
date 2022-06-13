# README  
## Problem  
*more detail, please see [Homework_2.pdf](./Homework_2.pdf).*
* Design a command reader `cmdReader`
  * Input
    * standard input (keyboard)
      * up/down/right/left arrows
      * back space
      * delete
      * Tab
      * Ctrl-a
      * Ctrl-e
      * Home
      * End
      * PgUp (Page Up)
      * PgDn (Page Down)
    * file input (dofile)
* Arguments
  * `[ ]` indicate optional arguments
  * `< >` indicate required arguments
* Keyboard Input Specification  
  *`^`: cursor*
  * Printable character
    ```plain
    cmd> read design abcd fyyh
                       ^
    type k33
    cmd> read design abk33cd fyyh
                          ^
    ```
  * Ctrl-a (LINE_BEGIN_KEY) or Home (HOME_KEY)
    ```plain
    cmd> read design abcd fyyh
                       ^
    press ctrl-a
    cmd> read design abcd fyyh
         ^
    ```
  * Ctrl-e (LINE_END_KEY) or End (END_KEY)
    ```plain
    cmd> read design abcd fyyh
                       ^
    press ctrl-e
    cmd> read design abcd fyyh
                              ^
    ```
  * Backspace (BACK_SPACE_KEY)
    ```plain
    cmd> read design abcd fyyh
                       ^
    press backspace
    cmd> read design acd fyyh
                      ^        
    ```
  * Delete key (DELETE_KEY)
    ```plain
    cmd> read design abcd fyyh
                       ^
    press delete
    cmd> read design abc fyyh
                       ^        
    ```
  * Tab key (TAB_KEY)
    * The tab position is defined by `TAB_POSITION` in `cmdParser.h`.
    * For example, let TAB_POSITION = 8, tab position can be 0, 8, 16… etc
    ```plain
    cmd> 01234567890123456
               ^
    press tab
    cmd> 012345  67890123456
                 ^
    ---
    cmd> 01234
              ^
    press tab
    cmd> 01234
                 ^
    ```
  * Enter (NEWLINE_KEY)
    * When the `enter` key is pressed
      * The leading and ending “spaces” in the line will be removed.
      * current line will be stored in the command history array(i.e. `vector<string> _history` in the `class CmdParser`).
      * Cursor will move to a new line.
    ```plain
    cmd>read design abk33cd fyyh
                                     ^
    type enter -> store "read design abk33cd fyyh"
    ---
    cmd>read design abk33cd fyyh
              ^
    type enter -> store "read design abk33cd fyyh"
    ---
    cmd>     read design abk33cd fyyh
                                          ^
    type enter -> store "read design abk33cd fyyh"
    ```
  * Up arrow (ARROW_UP_KEY) or PgUp (PG_UP_KEY)
    * When the `up arrow` key is pressed
      * Replace the current line with the previous string stored in the command history array.
    * When the `PgUp` key is pressed
      * Go back for `PG_OFFSET` lines at once (default `PG_OFFSET = 10`).
      * If there are **less than `PG_OFFSET` strings** before the current line in the command history array
        * Go back to the first string in the command history array.
  * Down arrow (ARROW_DOWN_KEY) or PgDn (PG_DN_KEY)
    * The rules are similar to those of `up arrow` and `PgUp`.
    ```plain
    [Example 10-1]
    1                cmd> 1       _history: {}
    <enter>          cmd>         _history: { "1" }
    2                cmd> 2       _history: { "1" }
    <enter>          cmd>         _history: { "1", "2" }
    <enter>          cmd>         _history: { "1", "2" }
    <up arrow>       cmd> 2       _history: { "1", "2", "" }
    0                cmd> 20      _history: { "1", "2", "" }
    <enter>          cmd>         _history: { "1", "2", "20" }
    
    [Example 10-2]
    1                cmd> 1       _history: {}
    <enter>          cmd>         _history: { "1" }
    2                cmd> 2       _history: { "1" }
    <enter>          cmd>         _history: { "1", "2" }
    <up arrow>       cmd> 2       _history: { "1", "2", "" }
    0                cmd> 20      _history: { "1", "2", "" }
    <up arrow>       cmd> 1       _history: { "1", "2", "" }
    <down arrow>     cmd> 2       _history: { "1", "2", "" }
    <down arrow>     cmd>         _history: { "1", "2" }
    <down arrow>     cmd>         _history: { "1", "2" }  // beep
    
    [Example 10-3]
    1                cmd> 1       _history: {}
    <enter>          cmd>         _history: { "1" }
    2                cmd> 2       _history: { "1" }
    <up arrow>       cmd> 1       _history: { "1", "2" }
    <down arrow>     cmd> 2       _history: { "1" }
    0                cmd> 20      _history: { "1" }
    <up arrow>       cmd> 1       _history: { "1", "20" }
    <down arrow>     cmd> 20      _history: { "1" }
    <down arrow>     cmd> 20      _history: { "1" } // beep
    ```
  * Right arrow (ARROW_RIGHT_KEY)
  * Left arrow (ARROW_LEFT_KEY)
* Others:
  * `-File` can be typed as `-file`, `-f`, `-F`, `-fi`..., etc.