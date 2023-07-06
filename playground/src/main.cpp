#include "playground.hpp"

int main()
{
	spk::Application app = spk::Application(L"Coucou", spk::Vector2Int(400, 400));
	
	return (app.run());
}
