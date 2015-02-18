#include "stdafx.h"

#include "CMemory.h"

#ifdef MRAY_MEMORY_MANAGMENT

#include <cassert>
#include <cstdlib>
#include <fstream>
namespace mray{

size_t CMemory::ms_uiNumNewCalls = 0;
size_t CMemory::ms_uiNumDeleteCalls = 0;
size_t CMemory::ms_uiMaxAllowedBytes = 0;
size_t CMemory::ms_uiNumBlocks = 0;
size_t CMemory::ms_uiNumBytes = 0;
CMemory::Block* CMemory::ms_pkHead = 0;
CMemory::Block* CMemory::ms_pkTail = 0;
bool CMemory::ms_bTrackSizes = false;
size_t CMemory::ms_uiMaxAllocatedBytes = 0;
size_t CMemory::ms_uiMaxBlockSize = 0;
size_t CMemory::ms_auiHistogram[32] =
{
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};

//----------------------------------------------------------------------------
void* CMemory::Allocate (size_t uiSize, char* acFile, unsigned int uiLine,
    bool bIsArray)
{
    ms_uiNumNewCalls++;

    // The 'assert' used to be enabled to trap attempts to allocate zero
    // bytes.  However, the DirectX function D3DXCheckNewDelete may pass in
    // a value of zero.
    // assert(uiSize > 0);

    // Allocate additional storage for the block header information.
    size_t uiExtendedSize = sizeof(Block) + uiSize;
    char* pcAddr = (char*)malloc(uiExtendedSize);

    // Save the allocation information.
    Block* pkBlock = (Block*)pcAddr;
    pkBlock->Size = uiSize;
    pkBlock->File = acFile;
    pkBlock->Line = uiLine;
    pkBlock->IsArray = bIsArray;
    InsertBlock(pkBlock);

    // Move the pointer to the start of what the user expects from 'new'.
    pcAddr += sizeof(Block);

    // Keep track of number of allocated blocks and bytes.
    ms_uiNumBlocks++;
    ms_uiNumBytes += uiSize;

    if (ms_uiMaxAllowedBytes > 0 && ms_uiNumBytes > ms_uiMaxAllowedBytes)
    {
        // The allocation has exceeded the maximum number of bytes.
        assert(false);
    }

    // Keep track of the maximum number of bytes allocated.
    if (ms_uiNumBytes > ms_uiMaxAllocatedBytes)
    {
        ms_uiMaxAllocatedBytes = ms_uiNumBytes;
    }

    // Keep track of the distribution of sizes for allocations.
    if (ms_bTrackSizes)
    {
        // Keep track of the largest block ever allocated.
        if (uiSize > ms_uiMaxBlockSize)
        {
            ms_uiMaxBlockSize = uiSize;
        }

        unsigned int uiTwoPowerI = 1;
        int i;
        for (i = 0; i <= 30; i++, uiTwoPowerI <<= 1)
        {
            if (uiSize <= uiTwoPowerI)
            {
                ms_auiHistogram[i]++;
                break;
            }
        }
        if (i == 31)
        {
            ms_auiHistogram[i]++;
        }
    }

    return (void*)pcAddr;
}
//----------------------------------------------------------------------------
void CMemory::Deallocate (char* pcAddr, bool bIsArray)
{
    ms_uiNumDeleteCalls++;

    if (!pcAddr)
    {
        return;
    }

    // Move the pointer to the start of the actual allocated block.
    pcAddr -= sizeof(Block);

    // Get the allocation information and remove the block.  The removal
    // only modifies the Prev and Next pointers, so the block information is
    // accessible after the call.
    Block* pkBlock = (Block*)pcAddr;
    RemoveBlock(pkBlock);

    // Keep track of number of allocated blocks and bytes.  If the number of
    // blocks is zero at this time, a delete has been called twice on the
    // same pointer.  If the number of bytes is too small at this time, some
    // internal problem has occurred within this class and needs to be
    // diagnosed.
    assert(ms_uiNumBlocks > 0 && ms_uiNumBytes >= pkBlock->Size);
    ms_uiNumBlocks--;
    ms_uiNumBytes -= pkBlock->Size;

    // Deallocate the CMemory block.
	try{
    free(pcAddr);
	}catch(...){
	}
}
//----------------------------------------------------------------------------
void CMemory::InsertBlock (Block* pkBlock)
{
    // New blocks are inserted at the tail of the doubly linked list.
    if (ms_pkTail)
    {
        pkBlock->Prev = ms_pkTail;
        pkBlock->Next = 0;
        ms_pkTail->Next = pkBlock;
        ms_pkTail = pkBlock;
    }
    else
    {
        pkBlock->Prev = 0;
        pkBlock->Next = 0;
        ms_pkHead = pkBlock;
        ms_pkTail = pkBlock;
    }
}
//----------------------------------------------------------------------------
void CMemory::RemoveBlock (Block* pkBlock)
{
    if (pkBlock->Prev)
    {
         pkBlock->Prev->Next = pkBlock->Next;
    }
    else
    {
        ms_pkHead = pkBlock->Next;
    }
    
    if (pkBlock->Next)
    {
        pkBlock->Next->Prev = pkBlock->Prev;
    }
    else
    {
        ms_pkTail = pkBlock->Prev;
    }
}
//----------------------------------------------------------------------------
void CMemory::GenerateReport (const char* acFilename)
{
    std::ofstream kOStr(acFilename);
    assert(kOStr);
    if (!kOStr)
    {
        return;
    }

    // Total calls.
    kOStr << "Total number of 'new' calls = "
        << (unsigned int)ms_uiNumNewCalls << std::endl;
    kOStr << "Total number of 'delete' calls = "
        << (unsigned int)ms_uiNumDeleteCalls << std::endl;
    kOStr << "Maximum number of allocated bytes = "
        << (unsigned int)ms_uiMaxAllocatedBytes << std::endl << std::endl;

    // Remaining counts.
    kOStr << "Remaining number of blocks = "
        << (unsigned int)ms_uiNumBlocks << std::endl;
    kOStr << "Remaining number of bytes  = "
        << (unsigned int)ms_uiNumBytes << std::endl << std::endl;

    // Count the blocks and bytes from known and unknown sources.
    size_t uiNumKnownBlocks = 0;
    size_t uiNumKnownBytes = 0;
    size_t uiNumUnknownBlocks = 0;
    size_t uiNumUnknownBytes = 0;
    Block* pkBlock = ms_pkHead;
    while (pkBlock)
    {
        if (pkBlock->File)
        {
            uiNumKnownBlocks++;
            uiNumKnownBytes += pkBlock->Size;
        }
        else
        {
            uiNumUnknownBlocks++;
            uiNumUnknownBytes += pkBlock->Size;
        }
        pkBlock = pkBlock->Next;
    }


    kOStr << "Remaining number of known blocks = "
        << (unsigned int)uiNumKnownBlocks << std::endl;
    kOStr << "Remaining number of known bytes  = "
        << (unsigned int)uiNumKnownBytes << std::endl << std::endl;

    kOStr << "Remaining number of unknown blocks = "
        << (unsigned int)uiNumUnknownBlocks << std::endl;
    kOStr << "Remaining number of unknown bytes  = "
        << (unsigned int)uiNumUnknownBytes << std::endl << std::endl;

    // Report the information for each block.
    pkBlock = ms_pkHead;
    size_t uiIndex = 0;
    while (pkBlock)
    {
        if (pkBlock->File)
        {
			kOStr << "block = " << (unsigned int)uiIndex << std::endl;
			kOStr << "size  = " << (unsigned int)pkBlock->Size << std::endl;
            kOStr << "file  = " << pkBlock->File << std::endl;
            kOStr << "line  = " << pkBlock->Line << std::endl;
			kOStr << "array = " << pkBlock->IsArray << std::endl << std::endl;
        }/*
        else
        {
            kOStr << "file  = unknown" << std::endl;
            kOStr << "line  = unknown" << std::endl;
        }*/
		pkBlock = pkBlock->Next;
        uiIndex++;
    }
    kOStr.close();

}
/*
void CMemory::clearMemory(){

	Block* ss=ms_pkHead;
	while(ss){
		Block* s2=ss->Next;
		ms_uiNumBlocks--;
		ms_uiNumBytes -= ss->Size;
		RemoveBlock(ss);
		ss=s2;
	}
}*/
inline size_t& CMemory::MaxAllowedBytes ()
{
    return ms_uiMaxAllowedBytes;
}
//----------------------------------------------------------------------------
inline bool& CMemory::TrackSizes ()
{
    return ms_bTrackSizes;
}
//----------------------------------------------------------------------------
inline size_t CMemory::GetNumNewCalls ()
{
    return ms_uiNumNewCalls;
}
//----------------------------------------------------------------------------
inline size_t CMemory::GetNumDeleteCalls ()
{
    return ms_uiNumDeleteCalls;
}
//----------------------------------------------------------------------------
inline size_t CMemory::GetNumBlocks ()
{
    return ms_uiNumBlocks;
}
//----------------------------------------------------------------------------
inline size_t CMemory::GetNumBytes ()
{
    return ms_uiNumBytes;
}
//----------------------------------------------------------------------------
inline size_t CMemory::GetMaxAllocatedBytes ()
{
    return ms_uiMaxAllocatedBytes;
}
//----------------------------------------------------------------------------
inline size_t CMemory::GetMaxBlockSize ()
{
    return ms_uiMaxBlockSize;
}
//----------------------------------------------------------------------------
inline size_t CMemory::GetHistogram (int i)
{
    if (0 <= i && i <= 31)
    {
        return ms_auiHistogram[i];
    }

    return 0;
}
//----------------------------------------------------------------------------
inline const CMemory::Block* CMemory::GetHead ()
{
    return ms_pkHead;
}
//----------------------------------------------------------------------------
inline const CMemory::Block* CMemory::GetTail ()
{
    return ms_pkTail;
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
void* CMemory::newElem (size_t uiSize)
{
	return mray::CMemory::Allocate(uiSize,0,0,false);
}
//----------------------------------------------------------------------------
void* CMemory::newArr(size_t uiSize)
{
    return mray::CMemory::Allocate(uiSize,0,0,true);
}
//----------------------------------------------------------------------------
void* CMemory::newElem (size_t uiSize, char* acFile, unsigned int uiLine)
{
    return mray::CMemory::Allocate(uiSize,acFile,uiLine,false);
}
//----------------------------------------------------------------------------
void* CMemory::newArr (size_t uiSize, char* acFile, unsigned int uiLine)
{
    return mray::CMemory::Allocate(uiSize,acFile,uiLine,true);
}
//----------------------------------------------------------------------------
void CMemory::deleteElem (void* pvAddr)
{
    mray::CMemory::Deallocate((char*)pvAddr,false);
}
//----------------------------------------------------------------------------
void CMemory::deleteArr (void* pvAddr)
{
    mray::CMemory::Deallocate((char*)pvAddr,true);
}
//----------------------------------------------------------------------------
void CMemory::deleteElem (void* pvAddr, char*, unsigned int)
{
    mray::CMemory::Deallocate((char*)pvAddr,false);
}
//----------------------------------------------------------------------------
void CMemory::deleteArr (void* pvAddr, char*, unsigned int)
{
    mray::CMemory::Deallocate((char*)pvAddr,true);
}

}

#endif