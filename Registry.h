#pragma once
#include <vector>
#include <bitset>
#include <queue>
#include "ComponentContainer.h"
#include <unordered_map>
#include <memory>
#include <bitset>

//#define XXH_INLINE_ALL
//#include "xxHash\xxhash.h"

namespace ECS
{
	constexpr uint64_t megaByteSize = 1024 * 1024;

	template <typename size_t maxSize = 100000, typename size_t memoryMegaByteSize = 1>
	class Registry
	{
	public:

		/// <summary>
		/// Creates a new entity
		/// </summary>
		/// <returns> a new entity </returns>
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
				entity = myEntitiesCount++;
			}
			

			if (myEntitiesCount > myValidEntities.size())
			{
				return -1;
			}

			myValidEntities[static_cast<size_t>(entity)] = true;

			return entity;
		}

		void Destroy(Entity aEntity)
		{
			myFreeIDs.push(aEntity);

			for (auto& it : myComponentContainers)
			{
				it.second->RemoveComponent(aEntity);
			}

			myValidEntities[static_cast<size_t>(aEntity)] = false;
		}

		/// <summary>
		/// Emplaces a new component. Does nothing if component already exists.
		/// </summary>
		/// <typeparam name="T">ComponentType</typeparam>
		/// <param name="aEntity"> Target of emplacement </param>
		/// <returns>Component reference</returns>
		template<typename T>
		T& Emplace(const Entity aEntity)
		{		
			const char* typeName = typeid(T).name();

			auto it = myComponentContainers.find(typeName);

			if (it == myComponentContainers.end())
			{
				auto* container = new ComponentContainer<T, memoryMegaByteSize>(myMemoryManager);

				myComponentContainers[typeName] = container;

				return container->Emplace(aEntity);

			}
			else
			{
				auto* container = dynamic_cast<ComponentContainer<T, memoryMegaByteSize>*>(it->second);

				T* component = container->TryGet(aEntity);

				if (component)
				{
					return *component;
				}
				else
				{
					return container->Emplace(aEntity);
				}

			}
		}

		/// <summary>
		/// Emplaces a new component. Replaces if component already exist.
		/// </summary>
		/// <typeparam name="T">ComponentType</typeparam>
		/// <param name="aEntity">  Target of emplacement </param>
		/// <returns>Component reference</returns>
		template<typename T>
		T& EmplaceOrReplace(const Entity aEntity)
		{
			const char* typeName = typeid(T).name();

			auto it = myComponentContainers.find(typeName);

			if (it == myComponentContainers.end())
			{
				auto* container = new ComponentContainer<T, memoryMegaByteSize>(myMemoryManager);

				myComponentContainers[typeName] = container;

				return container->Emplace(aEntity);

			}
			else
			{
				auto* container = dynamic_cast<ComponentContainer<T, memoryMegaByteSize>*>(it->second);
				
				return container->EmplaceOrReplace(aEntity);
			}
		}


		/// <summary>
		/// Gets the component. Emplaces a new component if
		/// there is none.
		/// </summary>
		/// <typeparam name="T">ComponentType</typeparam>
		/// <param name="aEntity">  Target of emplacement </param>
		/// <returns> Component reference </returns>
		template<typename T>
		T& GetOrEmplace(const Entity aEntity)
		{
			auto* container = TryGetContainer<T>();

			if (container)
			{
				T* component = container->TryGet(aEntity);

				if (component)
				{
					return *component;
				}
				else
				{
					return container->Emplace(aEntity);
				}
			}
			else
			{
				auto* newContainer = new ComponentContainer<T, memoryMegaByteSize>(myMemoryManager);

				const char* typeName = typeid(T).name();

				myComponentContainers[typeName] = newContainer;

				return newContainer->Emplace(aEntity);
			}
		}

		/// <summary>
		/// Tries to get the component
		/// </summary>
		/// <typeparam name="T">ComponentType</typeparam>
		/// <param name="aEntity"> Owner of component </param>
		/// <returns> A pointer to the component. Nullpointer if component cannot be found </returns>
		template<typename T>
		T* TryGet(const Entity aEntity) 
		{
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

		/// <summary>
		/// Gets the component. Should only be used when you are sure the entity has the component, otherwise you will
		/// get undefined behaviour. 
		/// </summary>
		/// <typeparam name="T">ComponentType</typeparam>
		/// <param name="aEntity"> Owner of component </param>
		/// <returns> Component reference </returns>
		template<typename T>
		T& Get(const Entity aEntity)
		{
			auto* container = GetContainer<T>();

			return *container->Get(aEntity);
		}

		/// <summary>
		/// Removes component from entity
		/// </summary>
		/// <typeparam name="T">ComponentType</typeparam>
		/// <param name="aEntity">Owner of component </param>
		/// <returns> Returns true if removal was successful, otherwise returns false </returns>
		template<typename T>
		bool Remove(Entity aEntity)
		{
			auto container = TryGetContainer<T>();

			if (container)
			{
				return container->RemoveComponent(aEntity);
			}
			else
			{
				return false;
			}
		}


		/// <summary>
		/// Gets a container of all components of the choosen type.
		/// </summary>
		/// <typeparam name="T">Type of component</typeparam>
		/// <returns>Component container reference </returns>
		template<typename T>
		ComponentContainer<T, memoryMegaByteSize>& GetView()
		{
			auto* container = TryGetContainer<T>();

			if (container == nullptr)
			{
				container = new ComponentContainer<T, memoryMegaByteSize>(myMemoryManager);

				const char* typeName = typeid(T).name();

				myComponentContainers[typeName] = container;
			}

			return *container;
		}
		


		/// <summary>
		/// Connects a function to be called when the choosen component is emplaced or replaced to any entity.
		/// </summary>
		/// <typeparam name="T">Type of component</typeparam>
		/// <typeparam name="U"></typeparam>
		/// <typeparam name="V"></typeparam>
		/// <param name="aFunction">Adress of a function to be connected </param>
		/// <param name="aInstance">Adress of the functions class instance </param>
		template<class T, class U, class V>
		void ConnectOnEmplace(U&& aFunction, V&& aInstance)
		{
			const char* typeName = typeid(T).name();

			auto it = myComponentContainers.find(typeName);

			if (it == myComponentContainers.end())
			{
				auto* container = new ComponentContainer<T, memoryMegaByteSize>(myMemoryManager);

				myComponentContainers[typeName] = container;

				//container->ConnectOnCreate(aFunction, aInstance);
				container->ConnectOnCreate(std::forward<U>(aFunction), std::forward<V>(aInstance));

			}
			else
			{
				auto* container = dynamic_cast<ComponentContainer<T, memoryMegaByteSize>*>(it->second);
				//container->ConnectOnCreate(aFunction, aInstance);
				container->ConnectOnCreate(std::forward<U>(aFunction), std::forward<V>(aInstance));
			}
		}

		/// <summary>
		/// Connects a function to be called when the choosen component is emplaced or replaced to any entity.
		/// </summary>
		/// <typeparam name="T">Type of component</typeparam>
		/// <typeparam name="U"></typeparam>
		/// <param name="aFunction">Adress of a function to be connected </param>
		template<class T, class U>
		void ConnectOnEmplace(U&& aFunction)
		{
			const char* typeName = typeid(T).name();

			auto it = myComponentContainers.find(typeName);

			if (it == myComponentContainers.end())
			{
				auto* container = new ComponentContainer<T, memoryMegaByteSize>(myMemoryManager);

				myComponentContainers[typeName] = container;

				container->ConnectOnCreate(std::forward<U>(aFunction));

			}
			else
			{
				auto* container = dynamic_cast<ComponentContainer<T, memoryMegaByteSize>*>(it->second);
				container->ConnectOnCreate(std::forward<U>(aFunction));
			}
		}

		/// <summary>
		/// Disconnects a function to be called when the choosen component is emplaced or replaced to any entity.
		/// </summary>
		/// <typeparam name="T">Type of component</typeparam>
		/// <typeparam name="U"></typeparam>
		/// <typeparam name="V"></typeparam>
		/// <param name="aFunction">Adress of a function to be disconnected </param>
		/// <param name="aInstance">Adress of the functions class instance </param>
		template<class T, class U, class V>
		void DisconnectOnEmplace(U&& aFunction, V&& aInstance)
		{
			const char* typeName = typeid(T).name();

			auto it = myComponentContainers.find(typeName);

			if (it != myComponentContainers.end())
			{
				auto* container = dynamic_cast<ComponentContainer<T, memoryMegaByteSize>*>(it->second);
				container->DisconnectOnCreate(std::forward<U>(aFunction), std::forward<V>(aInstance));
			}
		}

		/// <summary>
		/// Disconnects a function to be called when the choosen component is emplaced or replaced to any entity.
		/// </summary>
		/// <typeparam name="T">Type of component</typeparam>
		/// <typeparam name="U"></typeparam>
		/// <param name="aFunction">Adress of a function to be disconnected </param>
		template<class T, class U>
		void DisconnectOnEmplace(U&& aFunction)
		{
			const char* typeName = typeid(T).name();

			auto it = myComponentContainers.find(typeName);

			if (it != myComponentContainers.end())
			{
				auto* container = dynamic_cast<ComponentContainer<T, memoryMegaByteSize>*>(it->second);
				container->DisconnectOnCreate(std::forward<U>(aFunction));
			}
		}

		/// <summary>
		/// Connects a function to be called when the choosen component is removed from any entity.
		/// </summary>
		/// <typeparam name="T">Type of component</typeparam>
		/// <typeparam name="U"></typeparam>
		/// <typeparam name="V"></typeparam>
		/// <param name="aFunction">Adress of a function to be connected </param>
		/// <param name="aInstance">Adress of the functions class instance </param>
		template<class T, class U, class V>
		void ConnectOnRemove(U&& aFunction, V&& aInstance)
		{
			const char* typeName = typeid(T).name();

			auto it = myComponentContainers.find(typeName);

			if (it == myComponentContainers.end())
			{
				auto* container = new ComponentContainer<T, memoryMegaByteSize>(myMemoryManager);

				myComponentContainers[typeName] = container;

				container->ConnectOnRemove(std::forward<U>(aFunction), std::forward<V>(aInstance));

			}
			else
			{
				auto* container = dynamic_cast<ComponentContainer<T, memoryMegaByteSize>*>(it->second);
				container->ConnectOnRemove(std::forward<U>(aFunction), std::forward<V>(aInstance));
			}
		}

		/// <summary>
		/// Connects a function to be called when the choosen component is removed from any entity.
		/// </summary>
		/// <typeparam name="T">Type of component</typeparam>
		/// <typeparam name="U"></typeparam>
		/// <param name="aFunction">Adress of a function to be connected </param>
		template<class T, class U>
		void ConnectOnRemove(U&& aFunction)
		{
			const char* typeName = typeid(T).name();

			auto it = myComponentContainers.find(typeName);

			if (it == myComponentContainers.end())
			{
				auto* container = new ComponentContainer<T, memoryMegaByteSize>(myMemoryManager);

				myComponentContainers[typeName] = container;

				container->ConnectOnRemove(std::forward<U>(aFunction));

			}
			else
			{
				auto* container = dynamic_cast<ComponentContainer<T, memoryMegaByteSize>*>(it->second);
				container->ConnectOnRemove(std::forward<U>(aFunction));
			}
		}

		/// <summary>
		/// Disconnects a function to be called when the choosen component is removed from any entity.
		/// </summary>
		/// <typeparam name="T">Type of component</typeparam>
		/// <typeparam name="U"></typeparam>
		/// <typeparam name="V"></typeparam>
		/// <param name="aFunction">Adress of a function to be disconnected </param>
		/// <param name="aInstance">Adress of the functions class instance </param>
		template<class T, class U, class V>
		void DisconnectOnRemove(U&& aFunction, V&& aInstance)
		{
			const char* typeName = typeid(T).name();

			auto it = myComponentContainers.find(typeName);

			if (it != myComponentContainers.end())
			{
				auto* container = dynamic_cast<ComponentContainer<T, memoryMegaByteSize>*>(it->second);
				container->DisconnectOnRemove(std::forward<U>(aFunction), std::forward<V>(aInstance));
			}
		}

		/// <summary>
		/// Disconnects a function to be called when the choosen component is removed from any entity.
		/// </summary>
		/// <typeparam name="T">Type of component</typeparam>
		/// <typeparam name="U"></typeparam>
		/// <param name="aFunction">Adress of a function to be disconnected </param>
		template<class T, class U>
		void DisconnectOnRemove(U&& aFunction)
		{
			const char* typeName = typeid(T).name();

			auto it = myComponentContainers.find(typeName);

			if (it != myComponentContainers.end())
			{
				auto* container = dynamic_cast<ComponentContainer<T, memoryMegaByteSize>*>(it->second);
				container->DisconnectOnRemove(std::forward<U>(aFunction));
			}
		}


		/// <summary>
		/// Clears all the components and entities 
		/// </summary>
		/// <param name="aPreserveSignalConnections"> If true the signal connections is preserved </param>
		void Reset(const bool aPreserveSignalConnections = false)
		{
			myEntitiesCount = 0;


			std::queue<Entity> emptyQueue;
			std::swap(myFreeIDs, emptyQueue);

			if (aPreserveSignalConnections == false)
			{

				for (auto it : myComponentContainers)
				{
					delete it.second;
				}


				std::unordered_map<std::string, ComponentContainerInterface*> emptyMap;
				std::swap(myComponentContainers, emptyMap);

			}
			else
			{
				for (auto a : myComponentContainers)
				{
					a.second->Reset();
				}
			}
		}

		/// <summary>
		/// Checks weather a entity is valid or not
		/// </summary>
		/// <param name="aEntity"></param>
		/// <returns></returns>
		bool Valid(ECS::Entity aEntity)
		{
			if (aEntity < 0 || static_cast<size_t>(aEntity) + 1 > myValidEntities.size())
			{
				return false;
			}

			return myValidEntities[static_cast<size_t>(aEntity)];
		}




	private:
		template<typename T>
		ComponentContainer<T, memoryMegaByteSize>* TryGetContainer()
		{
			const char* typeName = typeid(T).name();
			ComponentContainer<T, memoryMegaByteSize>* container = nullptr;

			auto it = myComponentContainers.find(typeName);


			if (it != myComponentContainers.end())
			{
				container = dynamic_cast<ComponentContainer<T, memoryMegaByteSize>*>(it->second);
			}

			return container;
		}


		template<typename T>
		ComponentContainer<T, memoryMegaByteSize>* GetContainer()
		{
			const char* typeName = typeid(T).name();
			ComponentContainer<T, memoryMegaByteSize>* container = nullptr;

			auto it = myComponentContainers.find(typeName);


			if (it != myComponentContainers.end())
			{
				container = dynamic_cast<ComponentContainer<T, memoryMegaByteSize>*>(it->second);
			}

			return container;
		}


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


		std::bitset<maxSize> myValidEntities;


		MemoryManager myMemoryManager{ megaByteSize * memoryMegaByteSize };

		std::unordered_map<std::string, ComponentContainerInterface*> myComponentContainers{};
	};
}

