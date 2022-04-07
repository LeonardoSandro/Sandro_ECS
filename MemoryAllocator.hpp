#pragma once
#include "MemoryManager.hpp"

#include <limits>

#pragma warning(push)
#pragma warning( disable : 4100)


template <class T>
class MemoryAllocator
{
public:

	MemoryManager* myMM = nullptr;

public:

	using value_type = T;


	using pointer = T*;
	using const_pointer = const T*;

	using void_ponter = void*;
	using const_void_ponter = const void*;
	using size_type = size_t;
	using difference_type = std::ptrdiff_t;

	template <class U> 
	struct rebind
	{
		typedef MemoryAllocator<U> other;
	};

	explicit MemoryAllocator(MemoryManager& aMM) : myMM(&aMM) {};
	~MemoryAllocator() = default;

	template <class U>
	MemoryAllocator(const MemoryAllocator<U>& aOther): myMM(aOther.myMM)
	{
	};

	template <class U>
	MemoryAllocator<U>& operator=(const MemoryAllocator<U>& aOther)
	{
		myMM = aOther.myMM;
		return *this;
	}


	template <class U>
	bool operator== (const MemoryAllocator<U>& aMM)
	{
		return myMM == aMM.myMM;
	}

	pointer allocate(size_type aSize)
	{
		return reinterpret_cast<pointer>(myMM->Alloc(aSize * sizeof(T)));
	}

	void deallocate([[maybe_unused]] pointer aPtr, size_type aSize)
	{
		myMM->Free(reinterpret_cast<char*>(aPtr));
	}

	friend class MemoryManager;


};

#pragma warning(pop)


