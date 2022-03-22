#pragma once
#include <cstdint>

typedef void* EntityHandle ;
namespace ecs
{
	struct BaseComponent
	{
		static uint32_t NextID();
		EntityHandle entityID;
	};

	template<typename T>
	struct Component : public BaseComponent
	{
		static const uint32_t ID = BaseComponent::NextID();
	};


	template<typename T>
	const uint32_t Component<T>::ID(BaseComponent::NextID());

	template<typename T>
	struct TestComponent : public BaseECSComponent<TestComponent>
	{
		float x;
		float y;
	};

}

