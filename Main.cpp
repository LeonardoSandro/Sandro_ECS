#include "Registry.h"

#include <memory>
#include <vector>

#include "MemoryManager.h"
#include "MemoryAllocator.h"









struct A
{
	float a =102.f;
};

struct B
{
	float b = 3.f;
	int ghhgh = 1;
};


struct Transform 
{
	float x;
	float y;
	float z;

	
	
	// !! replace with actual velocity object
	float velocityX = 4;
	float velocityY;
	float velocityZ;
};




template <class T> using Allocator = MemoryAllocator<T>;
template <class T> using Vector = std::vector<T, MemoryAllocator<T>>;


struct Junk
{
	bool a;
	bool b;
	bool c;
	long long int junkInt;
};

void test(MemoryManager& aMM)
{

	//Vector<int> v{ Allocator<int>(mm) };
	Vector<int> v{ Allocator<int>(aMM) };
	//v.reserve(40);

	v.push_back(9);
	v.push_back(9);

	{
		//Vector<Junk> junkVector{ Allocator<Junk>(aMM) };

		//for (int i = 0; i < 4; ++i)
		//	junkVector.push_back(Junk());

		std::vector<int> testV;

		for (int i = 0; i < 34; ++i)
			testV.push_back(i);
	}


	for (int i = 0; i < 34; ++i)
		v.push_back(i);



	//Vector<int> v2 = std::move(v);
	//v = v2;

//	Vector<int> v3{ Allocator<int>(aMM) };
//	for (int i = 0; i < 10; ++i)
//		v.push_back(9);
}

void test2(MemoryManager& aMM)
{

	//Vector<int> v{ Allocator<int>(mm) };
	Vector<int> v{ Allocator<char>(aMM) };
	//v.reserve(40);

	v.push_back('A');
	v.push_back('A');


	for (int i = 0; i < 34; ++i)
		v.push_back(i);



	//Vector<int> v2 = std::move(v);
	//v = v2;

//	Vector<int> v3{ Allocator<int>(aMM) };
//	for (int i = 0; i < 10; ++i)
//		v.push_back(9);
}



int main()
{
	const std::size_t N = 1024;
	MemoryManager mm(N);

	//Allocator<int>* allocator = new Allocator<int>(mm);
	//Vector<int> v{ Allocator<int>(mm) };

	test(mm);

	//Vector<int> v{ *allocator };
	//v.reserve(10);
	//for (int i = 0; i < 2; ++i)
	//	v.push_back(2);

	test(mm);
	test2(mm);
	test(mm);
	test2(mm);
	test(mm);
	test2(mm);
	test(mm);
	test2(mm);
	test(mm);
	test2(mm);
	test(mm);
	test2(mm);
	test(mm);

	for (size_t i = 0; i < 10000; i++)
	{
		test2(mm);
		test(mm);

	}
	test2(mm);


	std::unique_ptr<ECS::Registry> registry = std::make_unique<ECS::Registry>();

	ECS::Entity entity0 = registry->Create();
	ECS::Entity entity1 = registry->Create();
	ECS::Entity entity2 = registry->Create();
	ECS::Entity entity3 = registry->Create();
	
	registry->Destroy(entity2);


	ECS::Entity entity4 = registry->Create();
	ECS::Entity entity5 = registry->Create();

	ECS::Entity nullTest = ECS::null;

	auto& transform = registry->Emplace<Transform>(entity0);
	auto& transform2 = registry->Emplace<Transform>(entity1);
	auto& componentA2 = registry->Emplace<A>(entity1);
	auto& componentA3 = registry->Emplace<A>(entity3);
	auto& componentA4 = registry->Emplace<A>(entity4);
	auto& componentB4 = registry->Emplace<B>(entity4);


	for (int i = 0; i < 1000; i++)
	{
		ECS::Entity entity = registry->Create();

		registry->Emplace<Transform>(entity);
	}


	auto* componentNULL = registry->TryGet<B>(entity1);
	auto* componentNOTNULL = registry->TryGet<A>(entity3);
	auto* componentNOTNULL2 = registry->TryGet<A>(entity4);

	auto& view = registry->GetView<Transform>();


	for (auto& componentWrapper : view.GetComponents())
	{
		auto& component = componentWrapper.GetComponent();

		component.x += component.velocityX;
	}

	auto* gagag = registry->TryGet<Transform>(entity1);

	if (componentNULL)
	{
		int a = 1;
		a = 2;
	}

	//MemoryManager memManager(640000);
	//memManager.TestRun();





}