#include "playground.hpp"

int main()
{
	spk::Application app(L"Playground", spk::Vector2Int(400, 400));

	spk::CentralNode centralNode;

	spk::RemoteNode nodeA;
	spk::Server serverA;

	spk::RemoteNode nodeB;
	spk::Server serverB;

	spk::Client clientA;
	spk::Client clientB;


	serverA.setNewConnectionCallback([&](const spk::Server::EmiterID& p_emiterID) {
		spk::cout << L"ServerA received a new connection ! - ID [" << p_emiterID << L"]" << std::endl;
		});
	serverB.setNewConnectionCallback([&](const spk::Server::EmiterID& p_emiterID) {
		spk::cout << L"ServerB received a new connection ! - ID [" << p_emiterID << L"]" << std::endl;
		});

	clientA.setOnMessageReceptionCallback(0, [&clientA](const spk::Message& p_msg) {
		spk::cout << L"Client A - Receiving message type 0" << std::endl;
		clientA.send(spk::Message(1));
		});
	clientA.setOnMessageReceptionCallback(1, [&clientA](const spk::Message& p_msg) {
		spk::cout << L"Client A - Receiving message type 1" << std::endl;
		clientA.send(spk::Message(2));
		});
	clientA.setOnMessageReceptionCallback(2, [&clientA](const spk::Message& p_msg) {
		spk::cout << L"Client A - Receiving final message type 2" << std::endl;
		});

	clientB.setOnMessageReceptionCallback(0, [&clientB](const spk::Message& p_msg) {
		spk::cout << L"Client B - Receiving message type 0" << std::endl;
		clientB.send(spk::Message(1));
		});
	clientB.setOnMessageReceptionCallback(1, [&clientB](const spk::Message& p_msg) {
		spk::cout << L"Client B - Receiving message type 1" << std::endl;
		clientB.send(spk::Message(2));
		});
	clientB.setOnMessageReceptionCallback(2, [&clientB](const spk::Message& p_msg) {
		spk::cout << L"Client B - Receiving final message type 2" << std::endl;
		});

	centralNode.redirectMessage(0, &nodeA);
	centralNode.redirectMessage(1, &nodeB);
	centralNode.redirectMessage(2, &nodeA);

	serverA.setOnMessageReceptionCallback(0, [&serverA](const spk::Server::EmiterID& p_emiterID, const spk::Message& p_msg) {
		spk::cout << L"ServerA - receive message type [" << p_msg.header().id() << L"] from emiter [" << p_emiterID << L"]" << std::endl;
		serverA.sendTo(p_emiterID, p_msg.createAwnser(0));
		});
	serverB.setOnMessageReceptionCallback(1, [&serverB](const spk::Server::EmiterID& p_emiterID, const spk::Message& p_msg) {
		spk::cout << L"ServerB - receive message type [" << p_msg.header().id() << L"] from emiter [" << p_emiterID << L"]" << std::endl;
		serverB.sendTo(p_emiterID, p_msg.createAwnser(1));
		});
	serverA.setOnMessageReceptionCallback(2, [&serverA](const spk::Server::EmiterID& p_emiterID, const spk::Message& p_msg) {
		spk::cout << L"ServerA - receive message type [" << p_msg.header().id() << L"] from emiter [" << p_emiterID << L"]" << std::endl;
		serverA.sendTo(p_emiterID, p_msg.createAwnser(2));
		});

	centralNode.start(12500);
	serverA.start(12501);
	serverB.start(12502);

	clientA.connect(L"127.0.0.1", 12500);
	clientB.connect(L"127.0.0.1", 12500);

	nodeA.connect(L"127.0.0.1", 12501);
	nodeB.connect(L"127.0.0.1", 12502);

	spk::Widget::CentralNode* CentralNode = app.addRootWidget<spk::Widget::CentralNode>(L"CentralNode");
	CentralNode->setCentralNode(&centralNode);
	CentralNode->activate();

	spk::Widget::Server* ServerNodeA = app.addRootWidget<spk::Widget::Server>(L"ServerNodeA");
	ServerNodeA->setServer(&serverA);
	ServerNodeA->activate();

	spk::Widget::Server* ServerNodeB = app.addRootWidget<spk::Widget::Server>(L"ServerNodeB");
	ServerNodeB->setServer(&serverB);
	ServerNodeB->activate();

	spk::Widget::RemoteNode* removeManagerA = app.addRootWidget<spk::Widget::RemoteNode>(L"RemoteNodeA");
	removeManagerA->setRemoteNode(&nodeA);
	removeManagerA->activate();

	spk::Widget::RemoteNode* removeManagerB = app.addRootWidget<spk::Widget::RemoteNode>(L"RemoteNodeB");
	removeManagerB->setRemoteNode(&nodeB);
	removeManagerB->activate();

	spk::Widget::Client* ClientA = app.addRootWidget<spk::Widget::Client>(L"ClientA");
	ClientA->setClient(&clientA);
	ClientA->activate();

	spk::Widget::Client* ClientB = app.addRootWidget<spk::Widget::Client>(L"ClientB");
	ClientB->setClient(&clientB);
	ClientB->activate();

	clientA.send(spk::Message(0));
	clientB.send(spk::Message(0));


	return (app.run());
}