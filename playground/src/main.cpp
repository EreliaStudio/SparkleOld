#include "playground.hpp"

int main()
{
	spk::Application app(L"Coucou", 400, spk::GraphicalAPI::OpenGL);
	spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);

	return (app.run());
};