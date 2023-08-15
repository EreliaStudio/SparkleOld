#include "playground.hpp"

class EmiterWidget : public spk::Widget::Interface
{
private:
	void _onGeometryChange();
	void _onRender();
	
	bool _onUpdate();

	spk::Network::Client* _client;
	spk::Network::Server* _server;

public:
	EmiterWidget(const std::wstring& p_name);
	~EmiterWidget();

	void setClient(spk::Network::Client* p_client);
	void setServer(spk::Network::Server* p_server);
};

void EmiterWidget::_onGeometryChange()
{

}

void EmiterWidget::_onRender()
{

}

bool EmiterWidget::_onUpdate()
{
	if (spk::Keyboard::instance()->inputStatus(spk::Keyboard::Z) == spk::InputState::Down)
	{
		static int nbMessage = 0;
		spk::cout << "Sending message : " << nbMessage << std::endl;
		spk::Network::Message p_msg(15);
		p_msg << std::wstring(L"Ceci est un test rigolo");

		nbMessage++;
		_client->send(p_msg);
	}
	return (false);
}

EmiterWidget::EmiterWidget(const std::wstring& p_name) : 
	spk::Widget::Interface(p_name)
{

}

EmiterWidget::~EmiterWidget()
{

}

void EmiterWidget::setClient(spk::Network::Client* p_client)
{
	_client = p_client;
}

void EmiterWidget::setServer(spk::Network::Server* p_server)
{
	_server = p_server;

	_server->setOnMessageReceptionCallback(15, [&](const spk::Network::Server::EmiterID& p_emiterID, const spk::Network::Message& p_msg){
		std::wstring messageString;

		p_msg >> messageString;

		spk::cout << "Message received :" << messageString << std::endl;
	});
}

int main()
{
	spk::Application app(L"Coucou", 400, spk::Application::Configuration(true, true));
	spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);

	spk::Widget::ServerManager *_serverManager = app.addRootWidget<spk::Widget::ServerManager>(L"ServerManager");
	_serverManager->setServer(new spk::Network::Server());
	_serverManager->server()->start(26500);
	_serverManager->activate();

	spk::Widget::ClientManager *_clientManager = app.addRootWidget<spk::Widget::ClientManager>(L"ClientManager");
	_clientManager->setClient(new spk::Network::Client());
	_clientManager->client()->connect(L"82.65.223.127", 26500);
	_clientManager->activate();

	EmiterWidget *_emiterWidget = app.addRootWidget<EmiterWidget>(L"ClientManager");
	_emiterWidget->setClient(_clientManager->client());
	_emiterWidget->setServer(_serverManager->server());
	_emiterWidget->activate();

	return (app.run());
};