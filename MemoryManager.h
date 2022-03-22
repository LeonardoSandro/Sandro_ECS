#pragma once
#include <stdio.h>
#include <assert.h>
#include <iostream>

//#define HEAP_CAPACITY 640000



#define TEST 2

// Linear allocation

constexpr int IS_ALLOCATED = 1;
constexpr int IS_END_OF_HEAP = 2;
constexpr int KB = 1024;
constexpr int MB = 1024 * 1024;
constexpr int GB = 1024 * 1024 * 1024;



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
	char* myHeap = nullptr;
	const uint64_t myHeapCapacity;
	//HeapChunk myHeapAlloced[HEAP_ALLOCED_CAPACITY] = { 0 };
	//HeapChunk myHeapFreed[HEAP_FREED_CAPACITY] = { 0 };

	size_t myHeapAllocedSize = 0;
	size_t myHeapSize = 0;



					
	//Block		flags (LSB)
	/*	|--------------|
		|Header    000 |
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



	enum Bit : int
	{
		Bit0 = 1, // 2^0, bit 0
		Bit1 = 2,  // 2^1, bit 1;
		Bit2 = 4  // 2^2, bit 2
	};




public:

	//void TestRun();
	//MemoryManager(uint64_t aHeapCapacity);

	//header* FindFirstFit(header aBlockSize);

	//void* Alloc(size_t aSize);




	//// O(Alloced)
	//void Free(void* aObjectToBeFreed);
	//void FreeOLD(void* aObjectToBeFreed);





	

	MemoryManager(uint64_t aHeapCapacity) : myHeapCapacity(aHeapCapacity)
	{
		if (aHeapCapacity == 0)
		{
			assert(false);
			return;
		}


		//myHeap = reinterpret_cast<char*>(malloc(aHeapCapacity));

		// The call to the constructor initializes the elements to 0
		myHeap = new char[myHeapCapacity]();




		header blockHeader = myHeapCapacity - sizeof(header);

		// Set the three LSB to zero in order to make the block size be eavenly divisible by 8
		blockHeader &= ~(Bit::Bit0 | Bit::Bit1 | Bit::Bit2);

		// Set the first LSB to zero to indicate the memory is free
		const int firstBitZero = 0;
		blockHeader &= ~(1UL << firstBitZero);

		//store the header at the start of the heap 
		memcpy(&myHeap[0], &blockHeader, sizeof(header));

		
		/////////////////////////
		// store a header at the end of the block in the heap
		const int blockSize = blockHeader & ~Bit::Bit0;


		header endOfHeapHeader = blockHeader;

		// Set the second LSB to 1 to indicate the end of heap
		endOfHeapHeader |= 1UL << 1;


		memcpy(&(myHeap[blockSize]), &endOfHeapHeader, sizeof(header));
		/////////////////////////


	}

	header* FindFirstFit(header aBlockSize)
	{
		int ptr = 0;

		bool foundMemory = false;
		while (foundMemory == false)
		{
			header* oldBlockHeader = reinterpret_cast<header*>(myHeap + ptr);

			const int noHeader = 0;
			if (*oldBlockHeader == 0)
			{
				return oldBlockHeader;
			}

			if ((*oldBlockHeader & Bit::Bit1) == IS_END_OF_HEAP)
			{
				const int blockSize = *oldBlockHeader & ~Bit::Bit0;

				assert(false && "Heap is full");
				return oldBlockHeader;
			}

			if ((*oldBlockHeader & Bit::Bit0) == IS_ALLOCATED)
			{
				const int oldBlockSize = *oldBlockHeader & ~Bit::Bit0;

				ptr += oldBlockSize;
			}
			else
			{
				if (aBlockSize < *oldBlockHeader)
				{
					// Do a split

					unsigned char* bytePtr = reinterpret_cast<unsigned char*>(oldBlockHeader);

					unsigned char* splitPosition = bytePtr + aBlockSize;
					header splitHeader = *oldBlockHeader - aBlockSize;

					// Set the first LSB to zero to indicate the memory is free
					const int firstBitZero = 0;
					splitHeader &= ~(1UL << firstBitZero);

					memcpy(splitPosition, &splitHeader, sizeof(header));

					return oldBlockHeader;
				}
				else if (aBlockSize > *oldBlockHeader)
				{
					const int oldBlockSize = *oldBlockHeader & ~Bit::Bit0;

					ptr += oldBlockSize;

					continue;
				}
				else
				{
					return oldBlockHeader;
				}
			}
		}

		assert(false && "No more heap storage available.");

		return nullptr;
		//header* blockHeader = reinterpret_cast<header*>(&(myHeap[0]));

		//do
		//{



		//} while ((*blockHeader & ~Bit::Bit0) == IS_ALLOCATED)

		//while 
		//{

		//	const int blockSize = *blockHeader & ~Bit::Bit0;

		//	auto ptr = myHeap[blockSize];
		//}

		//return -1;
	}


	void* Alloc(size_t aSize)
	{
		if (aSize == 0)
		{
			return nullptr;
		}

		//assert(myHeapSize + aSize + sizeof(header) * 2 <= myHeapCapacity);

		//////////////////
		// Stores the size of the final block
		int padding = 8;
		header blockHeader = aSize + padding + sizeof(header) * 2;
		/////////////////////////


		// Set the three LSB to zero in order to make the block size be eavenly divisible by 8
		blockHeader &= ~(Bit::Bit0 | Bit::Bit1 | Bit::Bit2);



		header* memoryLocation = FindFirstFit(blockHeader);




		// Set the first LSB to one to indicate the memory is allocated
		blockHeader |= Bit::Bit0;
		// 


		//store the header at the start of the block in the heap
		memcpy(memoryLocation, &blockHeader, sizeof(header));

		//Pointer to the "payload" area
		unsigned char* bytePtr = reinterpret_cast<unsigned char*>(memoryLocation);

		void* result = bytePtr + sizeof(header);

		/////////////////////////
		// store the header at the end of the block in the heap as a "footer"
		const int blockSize = blockHeader & ~Bit::Bit0;


		memcpy(bytePtr + blockSize - sizeof(header), &blockHeader, sizeof(header));
		/////////////////////////



		//myHeapSize += blockSize;




		//HeapChunk chunk = { result, aSize };
		//assert(myHeapAllocedSize < HEAP_ALLOCED_CAPACITY);

		//myHeapAlloced[myHeapAllocedSize++] = chunk;


		return result;
	}


	// O(Alloced)
	void Free(void* aObjectToBeFreed)
	{
		if (aObjectToBeFreed == nullptr)
		{
			return;
		}


		unsigned char* bytePtr = reinterpret_cast<unsigned char*>(aObjectToBeFreed);




		header* blockHeader = reinterpret_cast<header*>(bytePtr - sizeof(header));


		/////

		// exempel på självförklarande kod

		///////////


		if ((*blockHeader & Bit::Bit0) == IS_ALLOCATED)
		{
			const int blockSize = *blockHeader & ~Bit::Bit0;


			// Set the first LSB to zero to indicate the memory is free
			const int firstBitZero = 0;
			*blockHeader &= ~(1UL << firstBitZero);


			//Mark the footer as free as well
			bytePtr = reinterpret_cast<unsigned char*>(blockHeader);
			header* footer = reinterpret_cast<header*>(bytePtr + blockSize - sizeof(header));

			*footer &= ~(1UL << firstBitZero);

			return;


		}
		else
		{
			return;
		}

	}




	// O(Alloced)
	void FreeOLD(void* aObjectToBeFreed)
	{
		if (aObjectToBeFreed == nullptr)
		{
			return;
		}

		int ptr = 0;

		bool foundMemory = false;
		while (foundMemory == false)
		{

			header* blockHeader = reinterpret_cast<header*>(myHeap + ptr);
			/////

			// exempel på självförklarande kod

			const int noHeader = 0;
			if (*blockHeader == 0)
			{
				return;
			}
			///////////


			if ((*blockHeader & Bit::Bit0) == IS_ALLOCATED)
			{
				const int blockSize = *blockHeader & ~Bit::Bit0;

				if (aObjectToBeFreed == reinterpret_cast<void*>(myHeap + ptr + sizeof(header)))
				{
					// Set the first LSB to zero to indicate the memory is free
					const int firstBitZero = 0;
					*blockHeader &= ~(1UL << firstBitZero);


					//Mark the footer as free as well
					unsigned char* bytePtr = reinterpret_cast<unsigned char*>(blockHeader);
					header* footer = reinterpret_cast<header*>(bytePtr + blockSize - sizeof(header));

					*footer &= ~(1UL << firstBitZero);

					return;
				}


				ptr += blockSize;
			}
			else
			{
				return;
			}
		}
	}
};

