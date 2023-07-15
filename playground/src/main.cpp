#include "playground.hpp"

int main()
{
	spk::Application app(L"Playground", spk::Vector2Int(400, 400));
	spk::Server server;
	spk::Client client;

	spk::CentralNode _centralNode;
	
	
	spk::ServerManager* serverManager = app.addRootWidget<spk::ServerManager>(L"ServerManager");
	serverManager->setServer(&server);
	serverManager->server()->start(12500);
	serverManager->activate();

	spk::ClientManager* clientManager = app.addRootWidget<spk::ClientManager>(L"ClientManager");
	clientManager->setClient(&client);
	clientManager->client()->connect(L"127.0.0.1", 12500);
	clientManager->activate();

	clientManager->client()->send(spk::Message(0));

	return (app.run());
}