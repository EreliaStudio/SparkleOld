#include "network/spk_server.hpp"

namespace spk
{
	void Server::_treatMessage(const EmiterID &p_emiterID, const spk::Message &p_msg)
	{
		auto callbackIt = _onMessageReceptionCallbacks.find(p_msg.header().id());
		if (callbackIt != _onMessageReceptionCallbacks.end())
		{
			callbackIt->second(p_emiterID, p_msg);
		}
		else
		{
			if (_onUnknownMessageTypeCallback != nullptr)
				_onUnknownMessageTypeCallback(p_emiterID, p_msg);
		}
	}

	long long Server::_findValidID()
	{
		long long id;

		for (id = MINIMAL_ID_VALUE; _clients.contains(id) == true; id++)
			;

		return (id);
	}

	Server::Server() : _socketContextWorker(L"Server Socket")
	{
		_socketListeningContract = _socketContextWorker.addJob(L"Accepting new connection", [&]()
			{
				Socket newSocket;
				if (_Acceptor.accept(newSocket) == true)
				{
					EmiterID newId = _findValidID();
					_clients[newId] = newSocket;
					if (_onNewConnectionCallback != nullptr)
						_onNewConnectionCallback(newId);
				}
			});

		_readingIncomingMessageContract = _socketContextWorker.addJob(L"Reading incoming message", [&]()
			{
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

	Server::~Server()
	{
		_socketContextWorker.stop();
	}

	void Server::start(const size_t &p_serverPort)
	{
		_Acceptor.start(p_serverPort);
		_socketContextWorker.resume();
	}

	void Server::stop()
	{
		_socketContextWorker.pause();
		_Acceptor.stop();
	}

	void Server::treatMessages()
	{
		while (_messagesToTreat.empty() == false)
		{
			std::pair<EmiterID, spk::Message> pairMessageID = _messagesToTreat.pop_front();
			_treatMessage(pairMessageID.first, pairMessageID.second);
		}
	}

	void Server::sendTo(const Server::EmiterID& p_emiterID, const spk::Message &p_msg)
	{
		if (_clients.contains(p_emiterID) == false)
			spk::throwException(L"Emiter link to no client");
		_clients[p_emiterID].send(p_msg);
	}
}