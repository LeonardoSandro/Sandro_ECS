#pragma once
#include <vector>
#include <bitset>
#include <queue>
#include "Entity.hpp"
#include "ComponentContainer.h"
#include <unordered_map>
#include <memory>
#include <string>

//#define XXH_INLINE_ALL
//#include "xxHash\xxhash.h"


namespace ECS
{
	class Registry
	{
	public:
		Entity Create();
		void Destroy(Entity aEntity);

		template<typename T>
		std::weak_ptr<T> Emplace(Entity aEntity)
		{
			//const char* typeName = typeid(T).name();
			//XXH64_hash_t hash = XXH3_64bits(typeName, sizeof(char) * strlen(typeName) + 1);
			//
			XXH64_hash_t hash = TypeNameToHash<T>();
			//size_t typeName = typeid(T).hash_code();

			if (myComponentContainers.find(hash) == myComponentContainers.end())
			{
				myComponentContainers[hash] = std::make_shared<ComponentContainer<T>>();

	/*			std::shared_ptr<ComponentContainerInterface> Test = myComponentContainers[hash];
				std::shared_ptr<ComponentContainer<T>> derived = std::dynamic_pointer_cast<ComponentContainer<T>>(Test);*/


				//myComponentContainers.insert({typeName, static_cast<std::shared_ptr<ComponentContainerInterface>>(std::make_shared<ComponentContainer<T>>())});




				//std::pair <size_t, std::shared_ptr<ComponentContainerInterface>>(typeName, std::make_shared<ComponentContainer<T>>());

				//myComponentContainers.insert({typeName, std::make_shared<ComponentContainer<T>>()});
			}


			std::shared_ptr<ComponentContainer<T>> container = std::dynamic_pointer_cast<ComponentContainer<T>>(myComponentContainers[hash]);

			return container->AddComponent<T>(aEntity);
		}

		template<typename T>
		std::weak_ptr<T> TryGet(Entity aEntity)
		{
			XXH64_hash_t hash = TypeNameToHash<T>();

			if (myComponentContainers.find(hash) == myComponentContainers.end())
			{
				
			}
			else
			{
				// empty
				return std::weak_ptr<T>();
			}
		}


	private:

		//template<typename T>
		//XXH64_hash_t TypeNameToHash()
		//{
		//	const char* typeName = typeid(T).name();
		//	return XXH3_64bits(typeName, sizeof(char) * strlen(typeName) + 1);
		//}


		static constexpr int myMaxComponentCount = 32;

		std::vector<std::bitset<myMaxComponentCount>> myEntities;
		std::queue<Entity> myFreeIDs;

		std::unordered_map<std::string, std::shared_ptr<ComponentContainerInterface>> myComponentContainers{};
	};
}

