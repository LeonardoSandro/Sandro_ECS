#pragma once
#include <stdio.h>
#include <assert.h>
#include <iostream>

//#define HEAP_CAPACITY 640000



#define TEST 2

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

	void TestRun();
	MemoryManager(uint64_t aHeapCapacity);

	header* FindFirstFit(header aBlockSize);

	void* Alloc(size_t aSize);




	// O(Alloced)
	void Free(void* aObjectToBeFreed);
	void FreeOLD(void* aObjectToBeFreed);


};

