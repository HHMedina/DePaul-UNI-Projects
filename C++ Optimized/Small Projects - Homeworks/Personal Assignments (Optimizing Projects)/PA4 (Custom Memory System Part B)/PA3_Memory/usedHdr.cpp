#include "usedHdr.h"

usedHdr::usedHdr(int sizeBlock)
	:usedNext(0), usedPrev(0), blockSize(sizeBlock), blockType(0xAA), aboveBlockFree(false), pad0(0), pad1(0)
{
}