#include "Signal.h"
#include <string>

int F(int aInt)
{
	return aInt;
}

struct StructTest
{
	int G(const int& aInt) const
	{
		return aInt;
	}

	int H(const std::string&)
	{
		return 0;
	}
};


void Foo(int aInt, char aChar)
{	
	aInt;
	aChar;
}



void SinkAndSightTest::EstablishConnection(ListenerExample& aInstance)
{
	myECSSignal.Connect(&ListenerExample::Bar, &aInstance);
	myECSSignal.Connect(&ListenerExample2::Foo);
	myECSSignal.Connect(&Foo);

	myECSSignal.Publish(42, 'c');
	aInstance;

	//entt::sink connection{ mySignal };
	//connection.connect<&Foo>();
	//connection.connect<&ListenerExample::Bar>(aInstance);
}




