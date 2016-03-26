#ifndef MEM_H
#define MEM_H

// forward declare
struct headHdr;

#include "heapHdr.h"

#define HEAP_SIZE ( (50 * 1024) )  //50K

enum  memWarning{

	memFindFreeListInsertion_Success,
	memFindFreeListInsertion_Failure,
	memFindFreeListInsertion_EmptyList,
	memFindFreeListInsertion_ReturnedEnd,
	memFindFreeListInsertion_ReturnedHead,
	memTryToCombineDown_Failure,
	memTryToCombineDown_Success
};

class memSystem
{
public:
	memSystem();
	~memSystem();
	heapHdr *getHeap();
	memVoid dump();

	// implement these functions
	memVoid Free(memVoid * const data);
	memVoid *Malloc(const memU32 size);
	memVoid InitializeSystem();


private:
	heapHdr	*heap;
	char	*rawMem;

	//helper functions
	memVoid AddToUsedList(usedHdr*&);
	memVoid removeFromUsedList(usedHdr*&);
	memVoid AddToFreeList(freeHdr*&);
	memVoid removeFromFreeList(freeHdr*&);
	memVoid CoalesceFreeNodeDownwards(freeHdr*&);
	memVoid CoalesceFreeNodeUpwards(freeHdr*&);

	memVoid CombineUp(usedHdr*&);
	memWarning TryToCombineDown(usedHdr*&);

	memVoid ImpressSecretPointer(freeHdr*&);
	memWarning FindFreeListInsertionLocation(freeHdr*&, freeHdr*&);
};

#endif // mem.h