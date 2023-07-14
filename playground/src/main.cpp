#include "playground.hpp"

class NetworkAbstraction : public spk::AbstractWidget
{
private:
	spk::Server* _server;
	spk::Client* _client;

	void _onRender()
	{

	}
	void _onGeometryChange()
	{

	}

	bool _onUpdate()
	{		
		return (false);
	}

	void sendToClient(const size_t& p_msgID, const size_t& p_awnserID)
	{
		if (_server->_clients.size() == 0)
			return ;

		_server->sendTo(_server->_clients.begin()->second, spk::Message(p_awnserID));
	}
	void sendToServer(const size_t& p_msgID, const size_t& p_awnserID)
	{
		_client->send(spk::Message(p_awnserID));
	}
public:
	NetworkAbstraction(std::wstring p_name) : spk::AbstractWidget(p_name)
	{
		
	}

	void setServer(spk::Server* p_server)
	{
		_server = p_server;

		_server->setOnMessageReceptionCallback(0, [&](const spk::Server::ID& p_emiter, const spk::Message& p_msg){
			sendToClient(0, 0);
		});
		_server->setOnMessageReceptionCallback(1, [&](const spk::Server::ID& p_emiter, const spk::Message& p_msg){
			sendToClient(1, 1);
		});
		_server->setOnMessageReceptionCallback(2, [&](const spk::Server::ID& p_emiter, const spk::Message& p_msg){
			sendToClient(2, 2);
		});
		_server->setOnMessageReceptionCallback(3, [&](const spk::Server::ID& p_emiter, const spk::Message& p_msg){
			sendToClient(3, 3);
		});
		_server->setOnMessageReceptionCallback(4, [&](const spk::Server::ID& p_emiter, const spk::Message& p_msg){
			sendToClient(4, 4);
		});
		_server->setOnMessageReceptionCallback(5, [&](const spk::Server::ID& p_emiter, const spk::Message& p_msg){
			sendToClient(5, 5);
		});
	}

	void setClient(spk::Client* p_client)
	{
		_client = p_client;

		_client->setOnMessageReceptionCallback(0, [&](const spk::Message& p_msg){
			sendToServer(0, 1);
		});
		_client->setOnMessageReceptionCallback(1, [&](const spk::Message& p_msg){
			sendToServer(1, 2);
		});
		_client->setOnMessageReceptionCallback(2, [&](const spk::Message& p_msg){
			sendToServer(2, 3);
		});
		_client->setOnMessageReceptionCallback(3, [&](const spk::Message& p_msg){
			sendToServer(3, 4);
		});
		_client->setOnMessageReceptionCallback(4, [&](const spk::Message& p_msg){
			sendToServer(4, 5);
		});
		_client->setOnMessageReceptionCallback(5, [&](const spk::Message& p_msg){
			static int nbLoop = 0;
			spk::cout << "Last message received : Loop [" << nbLoop << "]" << std::endl;
			nbLoop++;
			sendToServer(5, 0);
		});
	}
};


int main()
{
	spk::Application app(L"Playground", spk::Vector2Int(400, 400));
	spk::Server server;
	spk::Client client;
	
	spk::ServerManager* serverManager = app.addRootWidget<spk::ServerManager>(L"ServerManager");
	serverManager->setServer(&server);
	serverManager->server()->start(12500);
	serverManager->activate();

	spk::ClientManager* clientManager = app.addRootWidget<spk::ClientManager>(L"ClientManager");
	clientManager->setClient(&client);
	clientManager->client()->connect(L"127.0.0.1", 12500);
	clientManager->activate();

	NetworkAbstraction* networkAbstraction = app.addRootWidget<NetworkAbstraction>(L"NetworkAbstractionWidget");
	networkAbstraction->setServer(serverManager->server());
	networkAbstraction->setClient(clientManager->client());
	networkAbstraction->activate();

	clientManager->client()->send(spk::Message(0));

	return (app.run());
}