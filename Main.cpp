#include "Registry.h"
#include <memory>

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



	auto componentA = registry->Emplace<Transform>(entity0);
	auto componentB = registry->Emplace<A>(entity4);

	auto componentNULL = registry->TryGet<B>(entity1);

	if (componentNULL.expired())
	{

	}

}