#include "BaseComponent.h"

namespace ecs
{
	static Entity componentID = 0;

	uint32_t BaseComponent::NextID()
	{
		return componentID++;
	}
}