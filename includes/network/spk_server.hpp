#pragma once

#include <map>
#include "threading/spk_persistent_worker.hpp"
#include "network/spk_message.hpp"
#include "network/spk_sink.hpp"
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

		Sink _sink;
		spk::ThreadSafeQueue<std::pair<ID, spk::Message>> _messagesToTreat;
		std::map<ID, Socket> _clients;
		std::map<spk::Message::Type, std::function<void(const ID&, const spk::Message&)>> _onMessageReceptionCallbacks;

		void _treatMessage(const ID& p_emiterID, const spk::Message& p_msg)
		{
			auto callbackIt = _onMessageReceptionCallbacks.find(p_msg.header().id());
			if(callbackIt != _onMessageReceptionCallbacks.end())
			{
				callbackIt->second(p_emiterID, p_msg);
			}
			else
			{
				spk::throwException(L"Callback not defined for message id [" + std::to_wstring(p_msg.header().id()) + L"]");
			}
		}

		long long _findValidID()
		{
			long long id;

			for (id = MINIMAL_ID_VALUE; _clients.contains(id) == true; id++);

			return (id);
		}

	public:
		Server() :
			_socketContextWorker(L"Server Socket")
		{
			_socketListeningContract = _socketContextWorker.addJob(L"Accepting new connection", [&](){
				Socket newSocket;
				if (_sink.accept(newSocket) == true)
				{
					ID newId = _findValidID();
					_clients[newId] = newSocket;
				}
			});

			_readingIncomingMessageContract = _socketContextWorker.addJob(L"Reading incoming message", [&](){
				spk::Message newMessage;

				for (auto it = _clients.begin(), next_it = it; it != _clients.end(); it = next_it)
				{
					if (it->second.isConnected() == true)
					{
						Socket::ReadResult readStatus = it->second.receive(newMessage);

						++next_it;

						switch (readStatus)
						{
							case Socket::ReadResult::Closed:					
								_clients.erase(it);
								break;
							case Socket::ReadResult::Success:
								_messagesToTreat.push_back(std::make_pair(it->first, newMessage));
								break;
						}
					}
				}
			});
			
			_socketContextWorker.start();
			_socketContextWorker.pause();
		}

		~Server()
		{
			_socketContextWorker.stop();
		}

		void start(const size_t& p_serverPort)
		{
			_sink.start(p_serverPort);
			_socketContextWorker.resume();
		}

		void stop()
		{
			_socketContextWorker.pause();
			_sink.stop();
		}

		void treatMessages()
		{
			while (_messagesToTreat.empty() == false)
			{
				std::pair<ID, spk::Message> pairMessageID = _messagesToTreat.pop_front();
				_treatMessage(pairMessageID.first, pairMessageID.second);
			}
		}

		template <typename Funct, typename ... Args>
		void setOnMessageReceptionCallback(const spk::Message::Type& p_id, Funct&& p_funct, Args&& ... p_args)
		{
			if (_onMessageReceptionCallbacks.contains(p_id) == true)
				spk::throwException(L"Callback already define for message ID [" + std::to_wstring(p_id) + L"]");
			_onMessageReceptionCallbacks[p_id] = std::bind(std::forward<Funct>(p_funct), std::placeholders::_1, std::placeholders::_2, std::forward<Args>(p_args)...);
		}

		void sendTo(Socket& p_target, const spk::Message& p_msg)
		{
			p_target.send(p_msg);
		}
	};
}