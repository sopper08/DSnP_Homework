# README

TASKInit
    1. init 0

TASKNew <-Random (size_t numMachines) | -Name (string name) (size_t load)>  
  * not init first (1)  
  * TASKNew -R numMachines  
  * TASKNew -N name load  
    * duplicated name (2)  

TASKRemove <-Random (size_t nMachines) | -Name (string name)>  
  * not init first (1)  
  * TASKRemove -R nMachines
    * bigger than exist numMachines (2)  
    * nMachines equal to 0 (3)  
  * TASKRemove -N name  
    * not exist (4)  
  
TASKQuery <(string name) | -HAsh | -HEap | -MINimum >  
  * TASKQuery name  
    * Removed name (1)  
  * TASKQuery < -HAsh | -HEap | -MINimum >

TASKAssign <(size_t load)> [-Repeat (size_t repeats)]
  * load = 0 (1)  
  * repeats = 0 (2)
