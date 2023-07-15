#pragma once

#include "network/spk_message.hpp"

namespace spk
{
	class CentralNode;

	class Node
	{
	private:
		CentralNode* _centralNode;

	public:
		Node() :
			_centralNode(nullptr)
		{

		}

		void link(CentralNode* p_centralNode)
		{
			_centralNode = p_centralNode;
		}

		virtual void send(const spk::Message& p_msg) = 0;

		virtual void receive(const spk::Message& p_msg)
		{
			_centralNode->returnMessage(p_msg);	
		}
	};

	class LocalNode : public spk::Node
	{
	private:
		std::map<spk::Message::Type, std::function<void(const spk::Message&)>> _onMessageReceptionCallbacks;

		std::function<void(const spk::Message&)> _onUnknowMessageReception = [&](const spk::Message& p_msg){
			spk::throwException(L"Callback already define for message ID [" + std::to_wstring(p_msg.header().id()) + L"]");
		};

	public:
		LocalNode()
		{

		}

		template <typename Funct, typename ... Args>
		void setOnMessageReceptionCallback(const spk::Message::Type& p_id, Funct&& p_funct, Args&& ... p_args)
		{
			if (_onMessageReceptionCallbacks.contains(p_id) == true)
				spk::throwException(L"Callback already defined for message type [" + std::to_wstring(p_id) + L"]");
			_onMessageReceptionCallbacks[p_id] = std::bind(std::forward<Funct>(p_funct), std::placeholders::_1, std::forward<Args>(p_args)...);
		}

		template <typename Funct, typename ... Args>
		void setUnknowMessageReceptionCallback(Funct&& p_funct, Args&& ... p_args)
		{
			_onUnknowMessageReception = std::bind(std::forward<Funct>(p_funct), std::placeholders::_1, std::forward<Args>(p_args)...);
		}

		void send(const spk::Message& p_msg)
		{

		}
	};

	class RemoteNode : public spk::Node
	{
	private:
		spk::Client _client;
		CentralNode* _centralNode;

	public:
		RemoteNode()
		{
			_client.setUnknowMessageReceptionCallback([&](const spk::Message& p_msg){	
				receive(p_msg);	
			});
		}

		void connect(const std::wstring& p_serverAddress, const size_t& p_serverPort)
		{
			_client.connect(p_serverAddress, p_serverPort);
		}

		void send(const spk::Message& p_msg)
		{
			_client.send(p_msg);
		}
	};

	class CentralNode
	{
	private:
		spk::Server _server;
		std::map<spk::Message::Type, Node*> _messagesRedirection;

	public:
		CentralNode()
		{
			_server.setUnknowMessageReceptionCallback([&](const spk::Server::EmiterID& p_id, const spk::Message& p_msg){
				if (_messagesRedirection.contains(p_msg.header().id()) == false)
				{
					spk::throwException(L"Message type [" + std::to_wstring(p_msg.header().id()) + L"] isn't connected to any node");
				}
				p_msg.header().setEmiter(p_id);
				_messagesRedirection[p_msg.header().id()]->send(p_id, p_msg);
			});
		}

		void start(size_t p_serverPort)
		{
			_server.start(p_serverPort);
		}

		void returnMessage(const spk::Message& p_msg)
		{
			_server.sendTo(p_msg.header().emiterID(), p_msg);
		}

		void redirectMessage(spk::Message::Type p_type, Node* p_destinationNode)
		{
			if (_messagesRedirection.contains(p_type) == true)
				spk::throwException(L"CentralNode has already a defined Node to message [" + std::to_wstring(p_type) + L"]");
			_messagesRedirection[p_type] = p_destinationNode;
			p_destinationNode->link(this);
		}
	};
}