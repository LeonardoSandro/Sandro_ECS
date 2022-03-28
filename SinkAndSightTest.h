#pragma once
//#include "ENTT\entt.hpp"
#include <vector>
#include <functional>







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
	class Sigh
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
			std::function<void(Args ...)> function = Bind(aFunction, &aInstance);

			myCallbacks.push_back(function);
			aFunction; aInstance;
		}

		template<class U>
		void Connect(U&& aFunction)
		{

			myCallbacks.push_back(aFunction);
		}
		

	private:

		// Deduce return type and parameters for the function
		template<typename A, typename B, typename C, typename... Params>
		std::function<A(Params...)> Bind(A(B::* function)(Params...), C pointer)
		{
			return [pointer, function](Params... params)->A {return (pointer->*function)(params...); };
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

	ECS::Sigh<int, char> myECSSignal;
	void EstablishConnection(ListenerExample& aInstance);
};

