#pragma once
#include <vector>
#include <bitset>
#include <queue>
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

		Entity Create()
		{
			Entity entity = {};

			if (myFreeIDs.size() > 0)
			{
			    entity = myFreeIDs.front();
			    myFreeIDs.pop();

			}
			else
			{
			    //myEntities.emplace_back();
			    //entity = static_cast<Entity>(myEntities.size() - 1);

				entity = myEntitiesCount++;
			}

			return entity;
		}
		//	


		void Destroy(Entity aEntity)
		{
			myFreeIDs.push(aEntity);
			//myEntities[aEntity].reset();


			for (auto& it : myComponentContainers)
			{
				it.second->EntityDestroyed(aEntity);
			}
		}




		template<typename T>
		T& Emplace(const Entity aEntity)
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
		T* TryGet(const Entity aEntity) 
		{
			//XXH64_hash_t hash = TypeNameToHash<T>();

			auto container = TryGetContainer<T>();

			if (container)
			{
				return container->TryGet(aEntity);
			}
			else
			{
				return nullptr;
			}
		}


		//template<typename T>
		//T* Get(const Entity aEntity)
		//{
		//	//XXH64_hash_t hash = TypeNameToHash<T>();

		//	auto container = TryGetContainer<T>();

		//	if (container)
		//	{
		//		return container->GetComponent(aEntity);
		//	}
		//	else
		//	{
		//		return nullptr;
		//	}
		//}


		//template<typename T>
		//std::vector<T>& View()
		//{
		//	const std::string typeName = typeid(T).name();
		//	ComponentContainer<T>* container = nullptr;

		//	auto it = myComponentContainers.find(typeName);


		//	if (it != myComponentContainers.end())
		//	{
		//		container = dynamic_cast<ComponentContainer<T>*>(it->second);
		//	}

		//	return container->GetComponents();
		//}


		template<typename T>
		ComponentContainer<T>* GetView()
		{
			return TryGetContainer<T>();
		}

		template<class T, class U>
		struct View2
		{
			ComponentContainer<T>* myFirst;
			ComponentContainer<U>* mySecond;
		};

		template<typename T, class U>
		View2<T, U> GetView()
		{
			View2<T, U> view;

			view.myFirst = TryGetContainer<T>();
			view.mySecond = TryGetContainer<U>();



			return view;
		}

		//template<class T, class U>
		//struct View3
		//{
		//	ComponentContainer<T>* myFirst;
		//	ComponentContainer<U>* mySecond;
		//	ComponentContainer<V>* myThird;
		//};

		//template<typename T, class U>
		//View3<T, U> GetView()
		//{



		//	return *TryGetContainer<T>();
		//}





		template<typename T>
		ComponentContainer<T>* TryGetContainer()
		{
			const std::string typeName = typeid(T).name();
			ComponentContainer<T>* container = nullptr;

			auto it = myComponentContainers.find(typeName);


			if (it != myComponentContainers.end())
			{
				container = dynamic_cast<ComponentContainer<T>*>(it->second);
			}

			return container;
		}



		template<typename T>
		ComponentContainer<T>* GetContainer()
		{
			const std::string typeName = typeid(T).name();
			ComponentContainer<T>* container = nullptr;

			auto it = myComponentContainers.find(typeName);


			if (it != myComponentContainers.end())
			{
				container = dynamic_cast<ComponentContainer<T>*>(it->second);
			}

			return container;
		}


		template<class T, class U, class V>
		void ConnectOnEmplace(U&& aFunction, V&& aInstance)
		{
			std::string typeName = typeid(T).name();

			auto it = myComponentContainers.find(typeName);

			if (it == myComponentContainers.end())
			{
				auto* container = new ComponentContainer<T>();

				myComponentContainers[typeName] = container;

				container->ConnectOnCreate(aFunction, aInstance);

			}
			else
			{
				auto* container = dynamic_cast<ComponentContainer<T>*>(it->second);
				container->ConnectOnCreate(aFunction, aInstance);
			}
		}

		template<class T, class U, class V>
		void ConnectOnDestroy(U&& aFunction, V&& aInstance)
		{
			std::string typeName = typeid(T).name();

			auto it = myComponentContainers.find(typeName);

			if (it == myComponentContainers.end())
			{
				auto* container = new ComponentContainer<T>();

				myComponentContainers[typeName] = container;

				container->ConnectOnDestroy(aFunction, aInstance);

			}
			else
			{
				auto* container = dynamic_cast<ComponentContainer<T>*>(it->second);
				container->ConnectOnDestroy(aFunction, aInstance);
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

		//std::vector<Entity> myEntities;

		Entity myEntitiesCount = 0;

		//std::vector<std::bitset<myMaxComponentCount>> myEntities;
		std::queue<Entity> myFreeIDs;

		std::unordered_map<std::string, ComponentContainerInterface*> myComponentContainers{};
	};
}

