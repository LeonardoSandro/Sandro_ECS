#pragma once
#include "MemoryManager.h"

#include <limits>

#pragma warning(push)
#pragma warning( disable : 4100)

template <class T>
class MemoryAllocator
{
public:
	// Memory managerns storlek sätts inte som ett templatevärde 

	MemoryManager& myMM;

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

	explicit MemoryAllocator(MemoryManager& aMM) : myMM(aMM) {};
	~MemoryAllocator() = default;



	//size_type max_size() const
	//{
	//	return std::numeric_limits<size_type>::max();
	//}


	//template<class U, class... Args>
	//void constuct(U* aPtr, Args&& ...args)
	//{
	//	new(aPtr) U(std::forward<Args>(args)...);
	//}



	template <class U>
	MemoryAllocator(const MemoryAllocator<U>& other): myMM(other.myMM)
	{
	};

	template <class U>
	MemoryAllocator<U>& operator=(const MemoryAllocator<U>&) = delete;

	pointer allocate(size_type aSize)
	{
		return reinterpret_cast<pointer>(myMM.Alloc(aSize * sizeof(T)));
	}

	void deallocate([[maybe_unused]] pointer aPtr, size_type aSize)
	{
		myMM.Free(reinterpret_cast<char*>(aPtr));
	}


	//template <class friendA, std::size_t friendASize, class friendB, std::size_t friendBSize>
	//friend bool operator==(const MemoryAllocator<friendA, friendASize>& aAllocatorA, const MemoryAllocator<friendB, friendBSize>& aAllocatorB);


	//template <class friendA, std::size_t friendASize, class friendB, std::size_t friendBSize>
	//friend bool operator!=(const MemoryAllocator<friendA, friendASize>& aAllocatorA, const MemoryAllocator<friendB, friendBSize>& aAllocatorB);


	friend class MemoryManager;


};
//
//template <class friendA, std::size_t friendASize, class friendB, std::size_t friendBSize>
//bool operator==(const MemoryAllocator<friendA, friendASize>& aAllocatorA, const MemoryAllocator<friendB, friendBSize>& aAllocatorB)
//{
//	return friendASize == friendBSize && &aAllocatorA.myMM == &aAllocatorB.myMM;
//}
//
//
//template <class friendA, std::size_t friendASize, class friendB, std::size_t friendBSize>
//bool operator!=(const MemoryAllocator<friendA, friendASize>& aAllocatorA, const MemoryAllocator<friendB, friendBSize>& aAllocatorB)
//{
//	return !(aAllocatorA == aAllocatorB);
//}
//

#pragma warning(pop)


