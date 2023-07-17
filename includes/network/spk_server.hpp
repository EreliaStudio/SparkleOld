#pragma once

#include <map>
#include "threading/spk_persistent_worker.hpp"
#include "network/spk_message.hpp"
#include "network/spk_acceptor.hpp"
#include "network/spk_socket.hpp"
#include "threading/spk_thread_safe_queue.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	class Server
	{
	public:
		using EmiterID = spk::Message::Header::EmiterID;
	public:
		static const EmiterID MINIMAL_ID_VALUE = 10000;
		spk::PersistentWorker _socketContextWorker;
		spk::ContractProvider::Contract _socketListeningContract;
		spk::ContractProvider::Contract _readingIncomingMessageContract;

		Acceptor _Acceptor;
		spk::ThreadSafeQueue<std::pair<EmiterID, spk::Message>> _messagesToTreat;
		std::map<EmiterID, Socket> _clients;
		std::map<spk::Message::Type, std::function<void(const EmiterID&, const spk::Message&)>> _onMessageReceptionCallbacks;
		std::function<void(const EmiterID&)> _onNewConnectionCallback = nullptr;
		std::function<void(const EmiterID&)> _onConnectionDisconnectionCallback = nullptr;
		std::function<void(const EmiterID&, const spk::Message&)> _onUnknownMessageTypeCallback = [&](const EmiterID& p_id, const spk::Message& p_msg){
			spk::throwException(L"Callback not defined for message id [" + std::to_wstring(p_msg.header().id()) + L"]");
		};

		void _treatMessage(const EmiterID& p_emiterID, const spk::Message& p_msg);

		long long _findValidID();

	public:
		Server();
		~Server();

		void start(const size_t& p_serverPort);
		void stop();

		void treatMessages();

		void setOnMessageReceptionCallback(const spk::Message::Type& p_id, std::function<void(const EmiterID&, const spk::Message&)> p_funct)
		{
			if (_onMessageReceptionCallbacks.contains(p_id) == true)
				spk::throwException(L"Callback already define for message type [" + std::to_wstring(p_id) + L"]");
			_onMessageReceptionCallbacks[p_id] = std::bind(p_funct, std::placeholders::_1, std::placeholders::_2);
		}
		template <typename ... Args>
		void setOnMessageReceptionCallback(const spk::Message::Type& p_id, std::function<void(const EmiterID&, const spk::Message&, Args...)> p_funct, Args&& ... p_args)
		{
			if (_onMessageReceptionCallbacks.contains(p_id) == true)
				spk::throwException(L"Callback already define for message type [" + std::to_wstring(p_id) + L"]");
			_onMessageReceptionCallbacks[p_id] = std::bind(p_funct, std::placeholders::_1, std::placeholders::_2, std::forward<Args>(p_args)...);
		}
		
		void setNewConnectionCallback(std::function<void(const EmiterID&)> p_funct)
		{
			_onNewConnectionCallback = std::bind(p_funct, std::placeholders::_1);
		}
		template <typename ... Args>
		void setNewConnectionCallback(std::function<void(const EmiterID&, Args...)> p_funct, Args&& ... p_args)
		{
			_onNewConnectionCallback = std::bind(p_funct, std::placeholders::_1, std::forward<Args>(p_args)...);
		}

		
		void setConnectionDisconnectionCallback(std::function<void(const EmiterID&)> p_funct)
		{
			_onConnectionDisconnectionCallback = std::bind(p_funct, std::placeholders::_1);
		}
		template <typename ... Args>
		void setConnectionDisconnectionCallback(std::function<void(const EmiterID&, Args...)> p_funct, Args&& ... p_args)
		{
			_onConnectionDisconnectionCallback = std::bind(p_funct, std::placeholders::_1, std::forward<Args>(p_args)...);
		}


		void setUnknowMessageReceptionCallback(std::function<void(const EmiterID&, const spk::Message&)> p_funct)
		{
			_onUnknownMessageTypeCallback = std::bind(p_funct, std::placeholders::_1, std::placeholders::_2);
		}

		template <typename ... Args>
		void setUnknowMessageReceptionCallback(std::function<void(const EmiterID&, const spk::Message&, Args...)> p_funct, Args&& ... p_args)
		{
			_onUnknownMessageTypeCallback = std::bind(p_funct, std::placeholders::_1, std::placeholders::_2, std::forward<Args>(p_args)...);
		}

		void sendTo(const Server::EmiterID& p_emiterID, const spk::Message& p_msg);
	};
}