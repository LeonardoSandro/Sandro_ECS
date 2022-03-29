#pragma once
#include "Entity.hpp"
#include <vector>
#include <memory>
#include "MemoryAllocator.h"
#include <string_view>
#include "Signal.h"



template <class T> using Allocator = MemoryAllocator<T>;
template <class T> using Vector = std::vector<T, MemoryAllocator<T>>;

namespace ECS
{
	constexpr uint64_t megaByteSize = 1024 * 1024;

	class ComponentContainerInterface
	{
	public:
		virtual ~ComponentContainerInterface() = default;
		virtual void EntityDestroyed(Entity aEntity) = 0;


		virtual void Reset() = 0;

	protected:
		// Sparse set.
		// The element index corresponds with the entity ID. The element contains a Index to the component belonging to the entity
		std::vector<int32_t> myComponentIndexes;
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

			
			ComponentWrapper& operator=(const ComponentWrapper& aWrapper)
			{
				if (this == &aWrapper)
				{
					return *this;
				}


				myComponent = aWrapper.myComponent;
				myEntity = aWrapper.myEntity;


				return *this;
			}

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
			Entity myEntity;
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


			myOnCreatedCallbacks.Publish(aEntity);

			//myViewIsUpdated = false;
			return myComponents.back().myComponent;
		}


		T* TryGet(Entity aEntity)
		{
			T* result = nullptr;

			if (myComponentIndexes.size() < static_cast<size_t>(aEntity + 1))
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





		T* Get(Entity aEntity)
		{
			assert(myComponentIndexes.size() >= static_cast<size_t>(aEntity + 1));

			T* result = nullptr;

			size_t entity = static_cast<size_t>(aEntity);
			int32_t componentIndex = myComponentIndexes[entity];

			assert(componentIndex != -1);

			result = &(myComponents[componentIndex].myComponent);

			return result;
		}


		void EntityDestroyed(Entity aEntity) 
		{
			// Try to remove the component
			if (myComponentIndexes.size() >= static_cast<size_t>(aEntity + 1))
			{
				int32_t componentIndex = myComponentIndexes[static_cast<int32_t>(aEntity)];

				if (componentIndex != ECS::null)
				{
					myComponentIndexes[static_cast<int32_t>(aEntity)] = ECS::null;

					if (componentIndex != myComponents.size() -1)
					{
						myComponents[componentIndex] = myComponents.back();

						// A other entity takes it's place
						Entity otherEntity = myComponents[componentIndex].GetEntity();

						myComponentIndexes[static_cast<int32_t>(otherEntity)] = componentIndex;
					}

					myComponents.pop_back();

					myOnDestroyCallbacks.Publish(aEntity);
				}
			}
		};


		Vector<ComponentWrapper>& GetComponents()
		{
			return myComponents;
		}



		template<class U, class V>
		void ConnectOnCreate(U&& aFunction, V&& aInstance)
		{
			myOnCreatedCallbacks.Connect(aFunction, aInstance);
		}

		template<class U>
		void ConnectOnCreate(U&& aFunction)
		{
			myOnCreatedCallbacks.Connect(aFunction);
		}

		template<class U, class V>
		void ConnectOnDestroy(U&& aFunction, V&& aInstance)
		{
			myOnDestroyCallbacks.Connect(aFunction, aInstance);
		}

		template<class U>
		void ConnectOnDestroy(U&& aFunction)
		{
			myOnDestroyCallbacks.Connect(aFunction);
		}


		// Preserves callbacks
		void Reset() 
		{
			myComponents.clear();
			myComponentIndexes.clear();
		}

	private:

		ECS::Signal<Entity> myOnCreatedCallbacks;
		ECS::Signal<Entity> myOnDestroyCallbacks;


		MemoryManager myMemoryManager{ megaByteSize };

		// Dense set
		Vector<ComponentWrapper> myComponents{ Allocator<ComponentWrapper>(myMemoryManager) };


		// For faster removal 
		//Entity myBackHolderEntity = ECS::null;



		//// For public use, fast iteration over all entities that owns component T.
		//std::vector<Entity> myView;
		//bool myViewIsUpdated = false;
	};




}

