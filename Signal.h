#pragma once
//#include "ENTT\entt.hpp"
#include <vector>
#include <functional>




// Glöm inte att lägga functionerna i public/private




struct ListenerExample
{
	void Bar(int aInt, char aChar)
	{
		aInt;
		aChar;

	}


	//void Bar()
	//{

	//}
};


struct ListenerExample2
{
	static void Foo(int aInt, char aChar)
	{
		aInt;
		aChar;
	}
};




namespace ECS
{
	// Kanske borde döpa den här till "Publisher"

	//template<typename T, typename ... Args>
	template<typename ... Args>
	class Signal
	{
	public:

		//template<typename... Args>
		void Publish(Args... aArgs)
		{	
			for (std::function<void(Args ...)>& callback : myCallbacks)
			{
				callback(aArgs...);

			}
		};


		template<class U, class V>
		void Connect(U && aFunction, V&& aInstance)
		{

			// Kanske går att klistra in Bind-koden direkt här istället?
			std::function<void(Args ...)> function = Bind(std::forward<U>(aFunction), std::forward<V>(aInstance));

			myCallbacks.push_back(function);
		}

		template<class U>
		void Connect(U&& aFunction)
		{
			myCallbacks.push_back(aFunction);
		}



		template<class U>
		void Disconnect(U&& aFunction)
		{
			for (size_t i = myCallbacks.size(); i > 0; i--)
			{
				if (GetAdress(myCallbacks[i -1]) == GetAdress(std::forward<U>(aFunction)))
				{
					myCallbacks[i - 1] = myCallbacks.back();
					myCallbacks.pop_back();
				}
			}
		}


		template<class U, class V>
		void Disconnect(U&& aFunction, V&& aInstance)
		{
			// Kanske går att klistra in Bind-koden direkt här istället?
			std::function<void(Args ...)> function = Bind(std::forward<U>(aFunction), std::forward<V>(aInstance));


			for (size_t i = myCallbacks.size(); i > 0; i--)
			{
				if (GetAdress(myCallbacks[i - 1]) == GetAdress(function))
				{
					myCallbacks[i - 1] = myCallbacks.back();
					myCallbacks.pop_back();
				}
			}
		}


	private:

		// Deduce return type and parameters for the function
		template<typename A, typename B, typename C, typename... Params>
		std::function<A(Params...)> Bind(A(B::* function)(Params...), C pointer)
		{
			return [pointer, function](Params... params)->A {return (pointer->*function)(params...); };
		}

		//size_t GetAdress(std::function<void(Args...)> aFunction)
		//{
		//	typedef void(functionType)(Args...);
		//	functionType** functionPointer = aFunction.template target<functionType*>();
		//	return (size_t)*functionPointer;
		//}


		long GetAdress(std::function<void(Args...)> aFunction)
		{
			return *(long*)(char*)&aFunction;
		}

		std::vector<std::function<void(Args...)>> myCallbacks;
	};
}



class SinkAndSightTest
{
public:


	void Init()
	{

		//ListenerExample instance;

		

		//EstablishConnection(instance);
	}


private:
	
	//entt::sigh<void(int, char)> mySignal;

	ECS::Signal<int, char> myECSSignal;
	void EstablishConnection(ListenerExample& aInstance);
};

