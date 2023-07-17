#pragma once

#include <map>
#include "network/spk_message.hpp"
#include "network/spk_acceptor.hpp"
#include "network/spk_socket.hpp"
#include "threading/spk_persistent_worker.hpp"
#include "threading/spk_thread_safe_queue.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	class Client
	{
	public:
		spk::PersistentWorker _socketContextWorker;
		spk::ContractProvider::Contract _readingSocketDataContract;

		spk::Socket _socket;
		spk::ThreadSafeQueue<spk::Message> _messagesToTreat;

		std::map<spk::Message::Type, std::function<void(const spk::Message&)>> _onMessageReceptionCallbacks;
		std::function<void(const spk::Message&)> _onUnknowMessageReception = [&](const spk::Message& p_msg){
			spk::throwException(L"Callback already define for message ID [" + std::to_wstring(p_msg.header().id()) + L"]");
		};

		void _treatMessage(const spk::Message& p_msg);

	public:
		Client();
		~Client();

		void connect(const std::wstring& p_serverAddress, const size_t& p_serverPort);
		void disconnect();

		void treatMessages();

		void setOnMessageReceptionCallback(const spk::Message::Type& p_id, std::function<void(const spk::Message&)> p_funct)
		{
			if (_onMessageReceptionCallbacks.contains(p_id) == true)
				spk::throwException(L"Callback already define for message type [" + std::to_wstring(p_id) + L"]");
			_onMessageReceptionCallbacks[p_id] = std::bind(p_funct, std::placeholders::_1);
		}
		template <typename ... Args>
		void setOnMessageReceptionCallback(const spk::Message::Type& p_id, std::function<void(const spk::Message&, Args...)> p_funct, Args&& ... p_args)
		{
			if (_onMessageReceptionCallbacks.contains(p_id) == true)
				spk::throwException(L"Callback already define for message type [" + std::to_wstring(p_id) + L"]");
			_onMessageReceptionCallbacks[p_id] = std::bind(p_funct, std::placeholders::_1, std::forward<Args>(p_args)...);
		}
		void setUnknowMessageReceptionCallback(std::function<void(const spk::Message&)> p_funct)
		{
			_onUnknowMessageReception = std::bind(p_funct, std::placeholders::_1);
		}
		template <typename ... Args>
		void setUnknowMessageReceptionCallback(std::function<void(const spk::Message&, Args...)> p_funct, Args&& ... p_args)
		{
			_onUnknowMessageReception = std::bind(p_funct, std::placeholders::_1, std::forward<Args>(p_args)...);
		}

		void send(const spk::Message& p_msg);
	};
}