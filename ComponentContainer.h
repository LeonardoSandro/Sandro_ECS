#pragma once
#include "Entity.hpp"
#include <vector>
#include <memory>

namespace ECS
{
	class ComponentContainerInterface
	{
	public:
		virtual ~ComponentContainerInterface() = default;
		virtual void EntityDestroyed(Entity aEntity) = 0;
	};

	template<typename T>
	class ComponentContainer : public ComponentContainerInterface
	{
	public:

		// This wrapper is a convinient way to hold both the component and the Entity-ID that it belongs to
		class ComponentWrapper
		{
		public:
			ComponentWrapper(T aComponent, Entity aEntity) : myComponent(aComponent), myEntity(aEntity) {};
			explicit ComponentWrapper(const ComponentWrapper& aWrapper) = default;

			T* GetComponent()
			{
				return &myComponent;
			}

			Entity GetEntity()
			{
				return myEntity;
			}

		private:
			T myComponent;
			const Entity myEntity;
			friend ComponentContainer;
		};

		ComponentContainer() = default;

		// Copy c:tor
		ComponentContainer(const ComponentContainer& aContainer) = delete;
		


		T& AddComponent(Entity aEntity)
		{
			myComponents.push_back({ T(), aEntity });
			size_t entity = static_cast<size_t>(aEntity);

			if (myComponentIndexes.size() < entity + 1)
			{
				int32_t unassigned = -1;
				myComponentIndexes.resize(entity + 1, unassigned);

				myComponentIndexes.back() = static_cast<int32_t>(myComponents.size() - 1);
			}
			else
			{
				myComponentIndexes[entity] = static_cast<int32_t>(myComponents.size() - 1);
			}

			myViewIsUpdated = false;
			return myComponents.back().myComponent;
		}


		T* TryGetComponent(Entity aEntity)
		{
			T* result = nullptr;

			if (myComponentIndexes.size() < aEntity - 1)
			{
				return result;
			}

			size_t entity = static_cast<size_t>(aEntity);
			int32_t componentIndex = myComponentIndexes[entity];
			
			if (componentIndex != -1)
			{
				result = &(myComponents[componentIndex].myComponent);
			}
		

			return result;
		}






		void EntityDestroyed(Entity aEntity) {};


		inline std::vector<ComponentWrapper>& GetComponents()
		{
			return myComponents;
		}

	private:



		std::vector<ComponentWrapper> myComponents;
		// Dense set

		// Sparse set.
		// The element index corresponds with the entity ID. The element contains a Index to the component belonging to the entity
		std::vector<int32_t> myComponentIndexes;


		// For public use, fast iteration over all entities that owns component T.
		std::vector<Entity> myView;
		bool myViewIsUpdated = false;
	};




}

