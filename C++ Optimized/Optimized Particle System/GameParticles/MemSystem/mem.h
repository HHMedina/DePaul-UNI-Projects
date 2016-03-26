#ifndef MEM_H
#define MEM_H

// forward declare
struct headHdr;

#include "heapHdr.h"

#define HEAP_SIZE ( ((30000 * 272) + (30010*16)) )  //50K

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
	


	

	// implement these functions
	static memVoid Free(memVoid * const data);
	static memVoid* Malloc(const memU32 size);
	static memVoid InitializeSystem();

	static void DestroyMe()
	{
		delete instance;
		instance = 0;
	}

private:
	static memSystem* instance;

	memSystem();
	~memSystem();

	heapHdr *getHeap();

	static memSystem& GetInstance()
	{
		if (!instance)
		{
			instance = new memSystem();
		}
		return *instance;
	}



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