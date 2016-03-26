#include "freeHdr.h"

freeHdr::freeHdr(int sizeBlock)
	:freeNext(0), freePrev(0), blockSize(sizeBlock), blockType(0xCC), pad0(0), pad1(0), pad2(0)
{
}
