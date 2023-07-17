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

	clientA.setOnMessageReceptionCallback(0, [&clientA](const spk::Message& p_msg){
		spk::cout << "Receiving message type 0" << std::endl;
		clientA.send(spk::Message(1));
	});
	clientA.setOnMessageReceptionCallback(1, [&clientA](const spk::Message& p_msg){
		spk::cout << "Receiving message type 1" << std::endl;
		clientA.send(spk::Message(2));
	});
	clientA.setOnMessageReceptionCallback(2, [&clientA](const spk::Message& p_msg){
		spk::cout << "Receiving final message type 2" << std::endl;
	});

	clientB.setOnMessageReceptionCallback(0, [&clientB](const spk::Message& p_msg){
		spk::cout << "Receiving message type 0" << std::endl;
		clientB.send(spk::Message(1));
	});
	clientB.setOnMessageReceptionCallback(1, [&clientB](const spk::Message& p_msg){
		spk::cout << "Receiving message type 1" << std::endl;
		clientB.send(spk::Message(2));
	});
	clientB.setOnMessageReceptionCallback(2, [&clientB](const spk::Message& p_msg){
		spk::cout << "Receiving final message type 2" << std::endl;
	});

	centralNode.redirectMessage(0, &nodeA);
	centralNode.redirectMessage(1, &nodeB);
	centralNode.redirectMessage(2, &nodeA);

	serverA.setOnMessageReceptionCallback(0, [&serverA](const spk::Server::EmiterID& p_emiter, const spk::Message& p_msg){
		serverA.sendTo(p_emiter, spk::Message(0));
	});
	serverB.setOnMessageReceptionCallback(1, [&serverB](const spk::Server::EmiterID& p_emiter, const spk::Message& p_msg){
		serverB.sendTo(p_emiter, spk::Message(1));
	});
	serverA.setOnMessageReceptionCallback(2, [&serverA](const spk::Server::EmiterID& p_emiter, const spk::Message& p_msg){
		serverA.sendTo(p_emiter, spk::Message(2));
	});

	centralNode.start(12500);
	serverA.start(12501);
	serverB.start(12502);

	clientA.connect(L"127.0.0.1", 12500);
	clientB.connect(L"127.0.0.1", 12500);

	nodeA.connect(L"127.0.0.1", 12501);
	nodeB.connect(L"127.0.0.1", 12502);

	spk::CentralNodeManager* centralNodeManager = app.addRootWidget<spk::CentralNodeManager>(L"CentralNodeManager");
	centralNodeManager->setCentralNode(&centralNode);
	centralNodeManager->activate(); 

	spk::ServerManager* serverManagerNodeA = app.addRootWidget<spk::ServerManager>(L"ServerNodeA");
	serverManagerNodeA->setServer(&serverA);
	serverManagerNodeA->activate();

	spk::ServerManager* serverManagerNodeB = app.addRootWidget<spk::ServerManager>(L"ServerNodeB");
	serverManagerNodeB->setServer(&serverB);
	serverManagerNodeB->activate();

	spk::RemoteNodeManager* removeManagerA = app.addRootWidget<spk::RemoteNodeManager>(L"RemoveNodeA");
	removeManagerA->setRemoteNode(&nodeA);
	removeManagerA->activate(); 

	spk::RemoteNodeManager* removeManagerB = app.addRootWidget<spk::RemoteNodeManager>(L"RemoteNodeB");
	removeManagerB->setRemoteNode(&nodeB);
	removeManagerB->activate(); 

	spk::ClientManager* clientManagerA = app.addRootWidget<spk::ClientManager>(L"ClientManagerA");
	clientManagerA->setClient(&clientA);
	clientManagerA->activate(); 

	spk::ClientManager* clientManagerB = app.addRootWidget<spk::ClientManager>(L"ClientManagerB");
	clientManagerB->setClient(&clientB);
	clientManagerB->activate(); 

	return (app.run());
}