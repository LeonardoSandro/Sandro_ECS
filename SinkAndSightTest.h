#pragma once
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


	//void Bar()
	//{

	//}
};




namespace ECS
{





	// Kanske borde döpa den här till "Publisher"

	//template<typename T, typename ... Args>
	template<typename T, typename ... Args>
	class Sigh
	{
	public:

		//using VoidFunction = void(*)(void*, Args...);

		//struct CallbackData
		//{
		//	VoidFunction myFunction;
		//	void* myClassInstance;
		//};




		//template<typename... Args>
		void Publish(Args... aArgs)
		{

			for (std::function<T(Args ...)>& callback : myCallbacks)
			{
				//callback.myFunction(callback.myClassInstance, aArgs...);

				callback(aArgs...);

			}
		};



		// Deduce return type and parameters for the function
		template<typename A, typename B, typename C, typename... Params>
		std::function<A(Params...)> Bind(A(B::*function)(Params...), C pointer)
		{
			return [pointer, function](Params... params)->A {return (pointer->*function)(params...); };
		}




		template<class U, class V>
		void Connect(U && aFunction, V&& aInstance)
		{

			// Kanske går att klistra in Bind-koden direkt här istället?
			std::function<T(Args ...)> function = Bind(aFunction, &aInstance);

			myCallbacks.push_back(function);
			aFunction; aInstance;
		}

		template<class U>
		void Connect(U&& aFunction)
		{

			myCallbacks.push_back(aFunction);



		}
		


		//template<class U>
		//void Connect(void (U::*aFunction)(Args...), U& aInstance)
		//{

		//	//std::function<T(Args...)> function;

		//	//if (aMember != nullptr)
		//	//{
		//	//	function = std::bínd(function, aMember);
		//	//}

		//	myCallbacks.push_back(aFunction);
		//}




	private:



		//std::vector<std::function<void(void)>> myCallbacks;

		std::vector<std::function<T(Args...)>> myCallbacks;
	};



	//template<class T>
	//class Listener : public ListenerBase
	//{
	//public:

	//private:

	//};
}






class SinkAndSightTest
{
public:

	void Init();

private:
	
	
	//entt::sigh<void(int, char)> mySignal;

	
	ECS::Sigh<void, int, char> myECSSignal;

	void EstablishConnection(ListenerExample& aInstance);
};

