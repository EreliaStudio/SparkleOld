#include "playground.hpp"

enum class Test
{
	A,
	B,
	C
};

int main()
{
	spk::Network::Message msg(15);
	spk::Network::Message msg2 = spk::Network::Message(Test::B);

	spk::Network::Message::Type value = msg.header().type();
	Test value1 = msg.header().typeAs<Test>();

	msg.header().setType(Test::A);

	Test value2 = msg.header().typeAs<Test>();

	return (0);
};