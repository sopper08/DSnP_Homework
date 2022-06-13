# README  

201911011734  
```C++
public:
   void* operator new(size_t t) { return (void*)(_memMgr->alloc(t)); }
   void* operator new[](size_t t) { return (void*)(_memMgr->allocArr(t)); }
```
t 不確定會收到什麼，應該要從講義的範例來看。  
> 會收到大小
但為啥還要 getArraySize，是為了找 recycle

201910311622  
---
在 RecycleList 中，需要串起一樣大小的 recycle memory，但是不知道該怎麼做。  
在 Test 中，template class 為 MemTestObj，但沒有發現可以串起來的 array。  
> 應該是用前 8 個 Bytes 來紀錄下一個位置

_recycleList[i]  _arrSize
              0         0
              1         1
              2         2
              3         3
              4         4
              5         5
              6         6
              7         7
              8         8
              9         9
             10        10
             11        11
             12        12
             13        13