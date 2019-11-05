#include <iostream>
using namespace std;

class MemoryBlock {
	char* _begin;
	char* _ptr;
	char* _end;
	MemoryBlock* _nextBlock;
};

class MemoryMgr {
	MemoryBlock* _activeBlock;
};

int main()
{
	
	return 0;
}
