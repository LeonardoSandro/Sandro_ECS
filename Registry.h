#pragma once
#include <vector>
#include <bitset>
#include <queue>
#include "ComponentContainer.h"
#include <unordered_map>
#include <memory>
#include <string>
#include "cc.h"

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
		T& Emplace(cc Entity aEntity)
		{
			//const char* typeName = typeid(T).name();
			//XXH64_hash_t hash = XXH3_64bits(typeName, sizeof(char) * strlen(typeName) + 1);
			//
			//XXH64_hash_t hash = TypeNameToHash<T>();
			//size_t typeName = typeid(T).hash_code();
			

			std::string typeName = typeid(T).name();

			auto it = myComponentContainers.find(typeName);

			if (it == myComponentContainers.end())
			{
				auto* container = new ComponentContainer<T>();

				myComponentContainers[typeName] = container;

	/*			std::shared_ptr<ComponentContainerInterface> Test = myComponentContainers[hash];
				std::shared_ptr<ComponentContainer<T>> derived = std::dynamic_pointer_cast<ComponentContainer<T>>(Test);*/


				//myComponentContainers.insert({typeName, static_cast<std::shared_ptr<ComponentContainerInterface>>(std::make_shared<ComponentContainer<T>>())});


				return container->AddComponent(aEntity);


				//std::pair <size_t, std::shared_ptr<ComponentContainerInterface>>(typeName, std::make_shared<ComponentContainer<T>>());

				//myComponentContainers.insert({typeName, std::make_shared<ComponentContainer<T>>()});
			}
			else
			{
				auto* container = dynamic_cast<ComponentContainer<T>*>(it->second);

				return container->AddComponent(aEntity);
			}




		}

		template<typename T>
		T* TryGet(cc Entity aEntity) 
		{
			//XXH64_hash_t hash = TypeNameToHash<T>();

			auto container = GetContainer<T>();

			if (container)
			{
				return container->TryGetComponent(aEntity);
			}
			else
			{
				return nullptr;
			}
		}

		//template<typename T>
		//std::vector<T>& View()
		//{
		//	cc std::string typeName = typeid(T).name();
		//	ComponentContainer<T>* container = nullptr;

		//	auto it = myComponentContainers.find(typeName);


		//	if (it != myComponentContainers.end())
		//	{
		//		container = dynamic_cast<ComponentContainer<T>*>(it->second);
		//	}

		//	return container->GetComponents();
		//}


		template<typename T>
		ComponentContainer<T>& View()
		{
			return *GetContainer<T>();
		}



		template<typename T>
		ComponentContainer<T>* GetContainer()
		{
			cc std::string typeName = typeid(T).name();
			ComponentContainer<T>* container = nullptr;

			auto it = myComponentContainers.find(typeName);


			if (it != myComponentContainers.end())
			{
				container = dynamic_cast<ComponentContainer<T>*>(it->second);
			}

			return container;
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

		std::unordered_map<std::string, ComponentContainerInterface*> myComponentContainers{};
	};
}

