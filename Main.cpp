#include "Registry.h"

#include <memory>

#include "MemoryManager.h"









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

int main()
{
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

	auto& view = registry->View<Transform>();
	for (auto& componentWrapper : view.GetComponents())
	{
		auto component = componentWrapper.GetComponent();

		component->x += component->velocityX;
	}

	auto* gagag = registry->TryGet<Transform>(entity1);

	if (componentNULL)
	{
		int a = 1;
		a = 2;
	}

	MemoryManager memManager;
	memManager.TestRun();
}