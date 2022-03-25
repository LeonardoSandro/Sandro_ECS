#include "SinkAndSightTest.h"

using std::string;

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

	int H(const string&)
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

//	std::function<void(void)> function = std::bind(&ListenerExample::Bar, aInstance);



	myECSSignal.Connect(&ListenerExample::Bar, aInstance);

	myECSSignal.Connect(&ListenerExample2::Foo);

	myECSSignal.Publish(42, 'c');
	aInstance;

	//entt::sink connection{ mySignal };
	//connection.connect<&Foo>();
	//connection.connect<&ListenerExample::Bar>(aInstance);
}




void SinkAndSightTest::Init()
{

	
	
	//entt::sink sink{ signal };


	//Function-ptr-test

	//void (SinkAndSightTest:: *blabla)(ListenerExample&);

	//blabla = &SinkAndSightTest::EstablishConnection;



	


	ListenerExample instance;

	

	EstablishConnection(instance);


	//mySignal.publish(42, 'c');



	//sink.disconnect(instance);

	//sink.disconnect();


	//// Delegate test //
	//entt::delegate<int(int)> delegate{};
	//delegate.connect<&F>();
	//StructTest instance;
	//delegate.connect<&StructTest::G>(instance);
	//delegate(4);
	//////////////////
}
