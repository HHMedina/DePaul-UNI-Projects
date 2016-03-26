#include <new>
#include <assert.h>
#include "mem.h"
#include "heapHdr.h"
#include "freeHdr.h"
#include "usedHdr.h"
#include "output.h"

#define ALIGNMENT_PAD (16)



memSystem::~memSystem()
{
	delete[] rawMem;
}


heapHdr *memSystem::getHeap()
{
	return heap;
}

memSystem::memSystem()
{
	// now initialize it.
	heap = 0;
	rawMem = 0;

	// create the memory block for the heap
	char *p_raw = new char[HEAP_SIZE + ALIGNMENT_PAD];

	// update the pointer
	rawMem = p_raw;

	// Correct pointer to make sure it's 16 byte aligned
	char *p_corrected = (char *)(((memU32)p_raw + (16 - 1)) & ~(16 - 1));

	// instantiate the heap header on the raw memory
	heapHdr *p = new(p_corrected)heapHdr(p_corrected);

	// update it
	heap = p;


}

memVoid memSystem::InitializeSystem()
{
	heapHdr* pHeap = this->getHeap();

	freeHdr* startFreeHdr = (freeHdr *)pHeap->stats.heapTopAddr;

	memU32 totalSize = (memU32)pHeap->stats.heapBottomAddr - (memU32)pHeap->stats.heapTopAddr;

	memU32 freeBlockSize = totalSize - sizeof(freeHdr);

	freeHdr * pFree = new(startFreeHdr)freeHdr(freeBlockSize);

	//fix heap variables
	pHeap->freeHead = pFree;

	pHeap->stats.currNumFreeBlocks = 1;
	pHeap->stats.currFreeMem = freeBlockSize;

	memU32 heapSize = totalSize + sizeof(heapHdr);

	pHeap->stats.sizeHeap = heapSize;
	pHeap->stats.sizeHeapHeader = sizeof(heapHdr);
}

memVoid memSystem::Free(memVoid * const data)
{

	heapHdr* pHeapHdr = getHeap();
	//this function should get rid of the desired information

	//data should be the address of the used block and not the header
	//therefore, to find the header, go up size of usedHdr
	usedHdr* pTargetUsedHdr = (usedHdr*)((char*)data - sizeof(usedHdr));



	//conversion of used block..
	pHeapHdr->stats.currFreeMem += pTargetUsedHdr->blockSize;//report freed space
	pHeapHdr->stats.currUsedMem -= pTargetUsedHdr->blockSize;//report loss of used space

	//adjust peaks
	if (pHeapHdr->stats.currUsedMem > pHeapHdr->stats.peakUsedMemory)
	{
		pHeapHdr->stats.peakUsedMemory = pHeapHdr->stats.currUsedMem;
	}

	if (pHeapHdr->stats.currNumUsedBlocks> pHeapHdr->stats.peakNumUsed)
	{
		pHeapHdr->stats.peakNumUsed = pHeapHdr->stats.currNumUsedBlocks;
	}


	//must remove the freed block from the used list 
	removeFromUsedList(pTargetUsedHdr);

	//
	bool isAboveTargetFree = pTargetUsedHdr->aboveBlockFree;

	//convert the used header to free header
	freeHdr* pConvertedFree = new(pTargetUsedHdr)freeHdr(pTargetUsedHdr->blockSize);

	//add to freed list
	AddToFreeList(pConvertedFree);

	//tell the block below this one that the above one is not free.
	usedHdr* pBlockBelowAllocated = (usedHdr*)((char*)pTargetUsedHdr + sizeof(usedHdr) + pTargetUsedHdr->blockSize);
	if (pBlockBelowAllocated < getHeap()->stats.heapBottomAddr && pBlockBelowAllocated->blockType == 0xAA)//is it within heap boundary and is it a used block
	{
		pBlockBelowAllocated->aboveBlockFree = true;
	}

	//place address of the free block on the lowest 4 bytes of the new free block
	ImpressSecretPointer(pConvertedFree);


	//try to coalesce the memory block below if there is one
	CoalesceFreeNodeDownwards(pConvertedFree);


	//pHeapHdr->freeHead = pConvertedFree;

	//if there is a free block above this, then coalesce it
	if (isAboveTargetFree){
		CoalesceFreeNodeUpwards(pConvertedFree);
	}

	//must add the freed block to the freed list (beginning)

	//adjust heap variables
	pHeapHdr->stats.currNumFreeBlocks++;
	pHeapHdr->stats.currNumUsedBlocks--;



	data;

}

memVoid * memSystem::Malloc(const memU32 size)
{

	memU32 adjSize = (((memU32)size + (16 - 1)) & ~(16 - 1));//align the requested data to 16 bytes

	heapHdr* pHeapHdr = getHeap();

	//check if the memory will fit within the remaining size of the heap
	if (pHeapHdr->stats.currFreeMem >= 0x20 && adjSize <= pHeapHdr->stats.currFreeMem)
	{

		//find a freehdr that can handle the size...
		freeHdr * pFreeIt;
		for (freeHdr * temp = pHeapHdr->freeHead; temp != 0; temp = temp->freeNext)
		{
			if (temp->blockSize >= adjSize)
			{
				pFreeIt = temp;
				break;
			}

		}

		//remove the freed node form the free list
		removeFromFreeList(pFreeIt);

		//find the new free size
		memU32 spaceDifference = (pFreeIt->blockSize) - adjSize;

		//switch it from the free to the used
		usedHdr* pUsedHdr = new(pFreeIt)usedHdr(adjSize);


		//tell the block below this one that the above one is not free.
		usedHdr* pBlockBelowAllocated = (usedHdr*)((char*)pUsedHdr + sizeof(usedHdr) + pUsedHdr->blockSize);
		if (pBlockBelowAllocated < getHeap()->stats.heapBottomAddr && pBlockBelowAllocated->blockType == 0xAA)//is it within heap boundary and is it a used block
		{
			pBlockBelowAllocated->aboveBlockFree = false;
		}

		//add to used list
		AddToUsedList(pUsedHdr);

		//update stats
		pHeapHdr->stats.currFreeMem -= (sizeof(usedHdr) - sizeof(freeHdr)) + adjSize;
		pHeapHdr->stats.currUsedMem += (sizeof(freeHdr) - sizeof(usedHdr)) + adjSize;
		pHeapHdr->stats.currNumUsedBlocks++;
		pHeapHdr->stats.currNumFreeBlocks--;

		if (pHeapHdr->stats.currUsedMem > pHeapHdr->stats.peakUsedMemory)
		{
			pHeapHdr->stats.peakUsedMemory = pHeapHdr->stats.currUsedMem;
		}

		if (pHeapHdr->stats.currNumUsedBlocks> pHeapHdr->stats.peakNumUsed)
		{
			pHeapHdr->stats.peakNumUsed = pHeapHdr->stats.currNumUsedBlocks;
		}


		//CREATE NEW FREEHEADER HERE
		if (spaceDifference >= sizeof(freeHdr)){//only make a free block and header if there is at least enough to hold the header
			//find the new freeHeader address location
			freeHdr* pNewFreeBlockLocation = (freeHdr*)((memU32)pUsedHdr + sizeof(usedHdr) + adjSize);
			//create new free block with remaining size
			spaceDifference -= sizeof(freeHdr);
			freeHdr* pSubdividedFreeHdr = new(pNewFreeBlockLocation)freeHdr(spaceDifference);//cahnged this!

			//adjust stats to account for new header.
			//pHeapHdr->stats.currFreeMem //already previously done
			pHeapHdr->stats.currNumFreeBlocks++;

			//add the node to the free list
			AddToFreeList(pSubdividedFreeHdr);

			//free header takes up space... account for this..
			pHeapHdr->stats.currFreeMem -= sizeof(freeHdr);



		}

		return ((char*)pUsedHdr + sizeof(usedHdr));//return the address to the used block
	}
	else{
		//assert(0 + "NO Memory available");
		return 0;//for now...
	}
}

memVoid memSystem::dump()
{

	fprintf(io::getHandle(), "\n------- DUMP -------------\n\n");

	fprintf(io::getHandle(), "heapStart: 0x%p     \nheapEnd:   0x%p \n\n", heap, heap->stats.heapBottomAddr);
	fprintf(io::getHandle(), "usedHead:  0x%p     \nfreeHead:  0x%p \n\n", heap->usedHead, heap->freeHead);

	fprintf(io::getHandle(), "Heap Hdr   s: %p  e: %p                            size: 0x%x \n", (memU32)heap->stats.heapTopAddr - sizeof(heapHdr), heap->stats.heapTopAddr, heap->stats.sizeHeapHeader);

	memU32 p = (memU32)heap->stats.heapTopAddr;

	char *type;
	char *typeHdr;

	while (p < (memU32)heap->stats.heapBottomAddr)
	{
		usedHdr *used = (usedHdr *)p;
		if (used->blockType == 0xAA)
		{
			typeHdr = "USED HDR ";
			type = "USED     ";
		}
		else
		{
			typeHdr = "FREE HDR ";
			type = "FREE     ";
		}

		memU32 hdrStart = (memU32)used;
		memU32 hdrEnd = (memU32)used + sizeof(usedHdr);
		fprintf(io::getHandle(), "%s  s: %p  e: %p  p: %p  n: %p  size: 0x%x    AF: %d \n", typeHdr, hdrStart, hdrEnd, used->usedPrev, used->usedNext, sizeof(usedHdr), used->aboveBlockFree);
		memU32 blkStart = hdrEnd;
		memU32 blkEnd = blkStart + used->blockSize;
		fprintf(io::getHandle(), "%s  s: %p  e: %p                            size: 0x%x \n", type, blkStart, blkEnd, used->blockSize);

		p = blkEnd;

	}
}

//added helper functions
memVoid memSystem::removeFromUsedList(usedHdr*& target)
{

	if (target->usedNext != 0){
		if (target->usedPrev != 0){//middle node operations
			target->usedPrev->usedNext = target->usedNext;
			target->usedNext->usedPrev = target->usedPrev;
		}
		else{//first node with next node operations
			getHeap()->usedHead = target->usedNext;
			getHeap()->usedHead->usedPrev = 0;

		}

	}
	else
	{
		if (target->usedPrev != 0){//last node  node operations
			target->usedPrev->usedNext = 0;
		}
		else//only node in list operations
		{
			getHeap()->usedHead = 0;
		}
	}

}

memVoid memSystem::AddToUsedList(usedHdr*& uHdrToAdd)
{
	//add to beginning
	if (getHeap()->usedHead != 0){
		uHdrToAdd->usedNext = getHeap()->usedHead;
		getHeap()->usedHead->usedPrev = uHdrToAdd;
		getHeap()->usedHead = uHdrToAdd;
		uHdrToAdd->usedPrev = 0;
	}
	else{
		getHeap()->usedHead = uHdrToAdd;
		uHdrToAdd->usedNext = 0;
		uHdrToAdd->usedPrev = 0;
	}
}

memVoid memSystem::removeFromFreeList(freeHdr*& target)
{


	if (target->freeNext != 0){
		if (target->freePrev != 0){//middle node operations
			target->freePrev->freeNext = target->freeNext;
			target->freeNext->freePrev = target->freePrev;
		}
		else{//first node with next node operations
			getHeap()->freeHead = target->freeNext;
			getHeap()->freeHead->freePrev = 0;

		}

	}
	else
	{
		if (target->freePrev != 0){//last node  node operations
			target->freePrev->freeNext = 0;
		}
		else//only node in list operations
		{
			getHeap()->freeHead = 0;
		}
	}

}

memVoid memSystem::AddToFreeList(freeHdr*& fHdrToAdd)
{


	if (getHeap()->freeHead != 0){

		freeHdr* pIt;
		memWarning result = FindFreeListInsertionLocation(fHdrToAdd, pIt);//search for insertion and return result of attempt

		if (result == memFindFreeListInsertion_Success){//middle node

			pIt->freePrev->freeNext = fHdrToAdd;
			fHdrToAdd->freePrev = pIt->freePrev;
			fHdrToAdd->freeNext = pIt;
			pIt->freePrev = fHdrToAdd;

		}
		else if (result == memFindFreeListInsertion_ReturnedEnd){//add to end
			pIt->freeNext = fHdrToAdd;
			fHdrToAdd->freePrev = pIt;
			fHdrToAdd->freeNext = 0;

		}
		else if (result == memFindFreeListInsertion_ReturnedHead){//
			pIt->freePrev = fHdrToAdd;
			fHdrToAdd->freeNext = pIt;
			fHdrToAdd->freePrev = 0;
			getHeap()->freeHead = fHdrToAdd;
		}

	}
	else{//add to the list as the only node

		getHeap()->freeHead = fHdrToAdd;
		fHdrToAdd->freePrev = 0;
		fHdrToAdd->freePrev = 0;

	}

}

memVoid memSystem::CoalesceFreeNodeDownwards(freeHdr*& target)
{

	//for now check underneaththis one to see if it needs to be merged.
	freeHdr* belowBlockAddr = (freeHdr*)((char*)target + sizeof(freeHdr) + target->blockSize);
	if (belowBlockAddr < getHeap()->stats.heapBottomAddr &&belowBlockAddr->blockType == 0xCC)
	{
		memU32 newFreeBlockSize = target->blockSize + belowBlockAddr->blockSize + sizeof(freeHdr);//added free hdr because only one header for each free block.

		removeFromFreeList(belowBlockAddr);
		removeFromFreeList(target);

		//set up new conjoined
		freeHdr* pConjoinedFreeBlock = new(target)freeHdr(newFreeBlockSize);
		ImpressSecretPointer(pConjoinedFreeBlock);

		getHeap()->stats.currNumFreeBlocks--;
		getHeap()->stats.currFreeMem += sizeof(freeHdr);
		AddToFreeList(pConjoinedFreeBlock);//changed this to try to fix pointers
	}


}

memVoid memSystem::CoalesceFreeNodeUpwards(freeHdr*& target)
{

	//get the address of the block above (should be one int away from the address in the negative direciton)
	unsigned int pPAddressofAboveBlock = *((unsigned int*)((char*)target - sizeof(unsigned int*)));
	freeHdr* newBlockLocation = (freeHdr*)pPAddressofAboveBlock;

	memU32 newSizeBlock = newBlockLocation->blockSize + target->blockSize + sizeof(freeHdr);

	//remove the frees from the list

	removeFromFreeList(target);
	removeFromFreeList(newBlockLocation);

	//create the new big block
	freeHdr* pConjoinedFreeBlock = new(newBlockLocation)freeHdr(newSizeBlock);
	ImpressSecretPointer(pConjoinedFreeBlock);

	AddToFreeList(pConjoinedFreeBlock);

	//adjust stats
	getHeap()->stats.currNumFreeBlocks--;
	getHeap()->stats.currFreeMem += sizeof(freeHdr);

}

memVoid memSystem::ImpressSecretPointer(freeHdr*& fHdr)
{
	//leave a address at the last 4 bytes of the free data block
	unsigned int* pSquirrelPointer = (unsigned int*)((char*)fHdr + sizeof(freeHdr) + fHdr->blockSize - sizeof(unsigned int*));
	pSquirrelPointer[0] = (unsigned int)(fHdr);

}

memWarning memSystem::FindFreeListInsertionLocation(freeHdr*& fHdrToAdd, freeHdr*& insertionNode)
{
	freeHdr* head = getHeap()->freeHead;

	freeHdr* pIt = head;
	freeHdr* pItPrev = head;
	while (pIt != 0){

		if (pIt > fHdrToAdd)
		{

			insertionNode = pIt;
			if (pIt == head)
			{
				return memFindFreeListInsertion_ReturnedHead;
			}
			return memWarning::memFindFreeListInsertion_Success;
		}
		else{
			pItPrev = pIt;
			pIt = pIt->freeNext;
		}
	}
	insertionNode = pItPrev; // return the end
	return memWarning::memFindFreeListInsertion_ReturnedEnd;
}
