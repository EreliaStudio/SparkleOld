#include "playground.hpp"

class EmiterWidget : public spk::Widget::Interface
{
private:
	void _onGeometryChange();
	void _onRender();
	
	bool _onUpdate();

	spk::Network::Client* _client;

public:
	EmiterWidget(const std::wstring& p_name);
	~EmiterWidget();

	void setClient(spk::Network::Client* p_client);
};

void EmiterWidget::_onGeometryChange()
{

}

void EmiterWidget::_onRender()
{

}

bool EmiterWidget::_onUpdate()
{
	if (spk::Keyboard::instance()->inputStatus(spk::Keyboard::Z) == spk::InputState::Pressed)
	{
		spk::cout << "Sending message" << std::endl;
		spk::Network::Message p_msg(15);
		p_msg << std::wstring(L"");

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
	_emiterWidget->activate();

	return (app.run());
};