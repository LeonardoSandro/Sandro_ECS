#include "BaseComponent.h"
#include "ECS\Entity.hpp"


namespace ecs
{
	static Entity componentID = 0;

	uint32_t BaseComponent::NextID()
	{
		return componentID++;
	}
}