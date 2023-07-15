#pragma once

#include <map>
#include "threading/spk_persistent_worker.hpp"
#include "network/spk_message.hpp"
#include "network/spk_acceptor.hpp"
#include "network/spk_socket.hpp"

namespace spk
{
	class Server
	{
	public:
		using ID = long long;
	public:
		static const long long MINIMAL_ID_VALUE = 10000;
		spk::PersistentWorker _socketContextWorker;
		spk::ContractProvider::Contract _socketListeningContract;
		spk::ContractProvider::Contract _readingIncomingMessageContract;

		Acceptor _Acceptor;
		spk::ThreadSafeQueue<std::pair<ID, spk::Message>> _messagesToTreat;
		std::map<ID, Socket> _clients;
		std::map<spk::Message::Type, std::function<void(const ID&, const spk::Message&)>> _onMessageReceptionCallbacks;

		void _treatMessage(const ID& p_emiterID, const spk::Message& p_msg);

		long long _findValidID();

	public:
		Server();
		~Server();

		void start(const size_t& p_serverPort);
		void stop();

		void treatMessages();

		template <typename Funct, typename ... Args>
		void setOnMessageReceptionCallback(const spk::Message::Type& p_id, Funct&& p_funct, Args&& ... p_args)
		{
			if (_onMessageReceptionCallbacks.contains(p_id) == true)
				spk::throwException(L"Callback already define for message ID [" + std::to_wstring(p_id) + L"]");
			_onMessageReceptionCallbacks[p_id] = std::bind(std::forward<Funct>(p_funct), std::placeholders::_1, std::placeholders::_2, std::forward<Args>(p_args)...);
		}

		void sendTo(Socket& p_target, const spk::Message& p_msg);
	};
}