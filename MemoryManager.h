#pragma once
#include <stdio.h>
#include <assert.h>
#include <iostream>

#define HEAP_CAPACITY 640000
#define HEAP_ALLOCED_CAPACITY 1024
#define HEAP_FREED_CAPACITY 1024


// Linear allocation





//
//struct HeapChunk
//{
//	void* myStart;
//	size_t mySize;
//};
//


// contains size + allocation flag in the LSB (least significant bit)


//struct Block
//{
//	
//	word myHeader;
//
//	char myPayload[0];
//};
//



class MemoryManager
{
	char myHeap[HEAP_CAPACITY] = {};
	//HeapChunk myHeapAlloced[HEAP_ALLOCED_CAPACITY] = { 0 };
	//HeapChunk myHeapFreed[HEAP_FREED_CAPACITY] = { 0 };

	size_t myHeapAllocedSize = 0;
	size_t myHeapSize = 0;


	//Block
	/*	|--------------|
		|Header        |
		|--------------|
		|Payload       |
		|              |
		|              |
		|--------------|
		|xxPaddingxxxxx|
		|xxxxxxxxxxxxxx|
		|--------------|
		|Header( A.K.A Footer)
		----------------*/


	typedef uint64_t header;

	enum Bits : int
	{
		Bit0 = 1, // 2^0, bit 0
		Bit1 = 2,  // 2^1, bit 1;
		Bit2 = 4  // 2^2, bit 2
	};


public:

	void TestRun()
	{

		//for (int i = 0; i < 100; i++)
		//{
		//	void* p = Alloc(i);

		//	//if (i % 2 == 0)
		//	//{
		//	//	Free(p);
		//	//}
		//}

		char* root = reinterpret_cast<char*>(Alloc(7));

		for (int i = 0; i < 26; i++)
		{
			root[i] = i + 'A';
		}

		char* root2 = reinterpret_cast<char*>(Alloc(26));

		for (int i = 0; i < 26; i++)
		{
			root2[i] = i + 'A';
		}


		int a = 1;

		DumpAllocedChunks();

		//Free(root);

	}

	void* Alloc(size_t aSize)
	{
		if (aSize == 0)
		{
			return nullptr;
		}
		
		assert(myHeapSize + aSize + sizeof(header) * 2 <= HEAP_CAPACITY);
		
		//////////////////
		// Stores the size of the final block
		int padding = 8;
		header blockHeader = aSize + padding + sizeof(header) * 2;
		/////////////////////////
		

		// Set the three LSB to zero in order to make the block size be eavenly divisible by 8
		blockHeader &= ~(Bits::Bit0 | Bits::Bit1 | Bits::Bit2);


		// Set the first LSB to one to indicate the memory is allocated
		blockHeader |= Bits::Bit0;

		//blockHeader = 0 - 187385;
		//store the header at the start of the block in the heap
		memcpy(&(myHeap[myHeapSize]), &blockHeader, sizeof(header));

		//Store the "payload"
		void* result = &(myHeap[myHeapSize + sizeof(header)]);

		/////////////////////////
		//store the header at the end of the block in the heap
		
		const int blockSize = blockHeader & ~Bits::Bit0;

		blockHeader = 0 - 1;
		memcpy(&(myHeap[myHeapSize + blockSize - sizeof(header)]), &blockHeader, sizeof(header));
		/////////////////////////

		myHeapSize += aSize + sizeof(header) * 2;

		//HeapChunk chunk = { result, aSize };
		//assert(myHeapAllocedSize < HEAP_ALLOCED_CAPACITY);

		//myHeapAlloced[myHeapAllocedSize++] = chunk;

		char* resultChar = reinterpret_cast<char*>(result);
		for (int i = 0; i < 7; i++)
		{
			resultChar[i] = i + 'A';
		}

		return result;
	}


	void DumpAllocedChunks()
	{
		//std::cout << "Allocated chunks: " << myHeapAllocedSize << "\n";

		//for (auto& chunk : myHeapAlloced)
		//{
		//	std::cout << "Start: " << chunk.myStart << " Size: " << chunk.mySize << "\n";
		//}
	}


	// O(Alloced)
	void Free(void* aPtr)
	{
		if (aPtr == nullptr)
		{
			return;
		}


		for (size_t i = 0; i < myHeapAllocedSize; i++)
		{
			//if (myHeapAlloced[i].myStart == aPtr)
			//{

			//}
		}






		//assert(false && "TODO");
	}

	void Collect()
	{
		assert(false && "TODO");
	}
};

