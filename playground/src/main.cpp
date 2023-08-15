#include "playground.hpp"

int main()
{
	spk::Application app(L"Coucou", 400, spk::Application::Configuration(true, true));

	spk::Widget::ServerManager *_serverManager = app.addRootWidget<spk::Widget::ServerManager>(L"ServerManager");
	_serverManager->setServer(new spk::Network::Server());
	_serverManager->server()->start(26500);
	_serverManager->activate();

	spk::Widget::ClientManager *_clientManager = app.addRootWidget<spk::Widget::ClientManager>(L"ClientManager");
	_clientManager->setClient(new spk::Network::Client());
	_clientManager->client()->connect(L"82.65.223.127", 26500);
	_clientManager->activate();

	spk::Network::Message p_msg(15);
	p_msg << std::wstring(L"Coucou");

	_clientManager->client()->send(p_msg);

	return (app.run());
};