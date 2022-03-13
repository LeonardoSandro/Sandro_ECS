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
		template<typename T>
		std::weak_ptr<T> AddComponent(Entity aEntity)
		{
			myComponents.push_back(std::make_shared<T>());
			size_t entity = static_cast<size_t>(aEntity);

			if (myIndexes.size() < entity + 1)
			{
				int32_t unassigned = -1;
				myIndexes.resize(entity + 1, unassigned);

				myIndexes.back() = entity;
			}
			else
			{
				myIndexes[entity] = myComponents.size() - 1;
			}


			return myComponents.back();
		}


		template<typename T>
		std::weak_ptr<T> TryGetComponent(Entity aEntity)
		{
			size_t entity = static_cast<size_t>(aEntity);


			
		}


		void EntityDestroyed(Entity aEntity) {};


	private:
		// Dense set
		std::vector<std::shared_ptr<T>> myComponents;

		// Sparse set.
		// The element index corresponds with the entity ID. The element contains a Index to the component belonging to the entity
		std::vector<int32_t> myIndexes;
	};




}

