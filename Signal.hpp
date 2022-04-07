#pragma once
#include <vector>
#include <functional>

namespace ECS
{
	template<typename ... Args>
	class Signal
	{
	public:

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

		long GetAdress(std::function<void(Args...)> aFunction)
		{
			return *(long*)(char*)&aFunction;
		}

		std::vector<std::function<void(Args...)>> myCallbacks;
	};
}


