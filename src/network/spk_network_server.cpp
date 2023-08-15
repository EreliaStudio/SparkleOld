#include "network/spk_network_server.hpp"

namespace spk::Network
{
	void Server::_treatMessage(const EmiterID& p_emiterID, const spk::Network::Message& p_msg)
	{
		auto callbackIt = _onMessageReceptionCallbacks.find(p_msg.header().type());
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
					_clients[newId] = std::move(newSocket);
					if (_onNewConnectionCallback != nullptr)
					{
						_onNewConnectionCallback(newId);
					}
				}
			});

		_readingIncomingMessageContract = _socketContextWorker.addJob(L"Reading incoming message", [&]()
			{
				struct timeval timeout;
				timeout.tv_sec = 5;
				timeout.tv_usec = 0;

				int activity = select(spk::Network::Object::maxFD() + 1, &spk::Network::Object::readingFDs(), &spk::Network::Object::writingFDs(), nullptr, &timeout);

				if (activity == SOCKET_ERROR)
				{
					//In case the sockets stored in the readingFDs failed.
				}
				else if (activity == 0)
				{
					return ;
				}
				else
				{
					spk::Network::Message newMessage;

					for (auto it = _clients.begin(), next_it = it; it != _clients.end(); it = next_it)
					{
						if (it->second.isConnected() == true)
						{
							Socket::ReadResult readStatus = Socket::ReadResult::NothingToRead;
							
							try
							{
								readStatus = it->second.receive(newMessage);
							}
							catch(...)
							{
								readStatus = Socket::ReadResult::Closed;
							}

							++next_it;

							switch (readStatus)
							{
							case Socket::ReadResult::Timeout:
							case Socket::ReadResult::Closed:
								if (_onConnectionDisconnectionCallback != nullptr)
									_onConnectionDisconnectionCallback(it->first);
								_clients.erase(it);
								break;
							case Socket::ReadResult::Success:
								_messagesToTreat.push_back(std::make_pair(it->first, newMessage));
								break;
							}
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

	void Server::start(const size_t& p_serverPort)
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
			std::pair<EmiterID, spk::Network::Message> pairMessageID = _messagesToTreat.pop_front();
			_treatMessage(pairMessageID.first, pairMessageID.second);
		}
	}

	void Server::setNewConnectionCallback(std::function<void(const EmiterID&)> p_funct)
	{
		_onNewConnectionCallback = std::bind(p_funct, std::placeholders::_1);
	}

	void Server::setConnectionDisconnectionCallback(std::function<void(const EmiterID&)> p_funct)
	{
		_onConnectionDisconnectionCallback = std::bind(p_funct, std::placeholders::_1);
	}

	void Server::setUnknowMessageReceptionCallback(std::function<void(const EmiterID&, const spk::Network::Message&)> p_funct)
	{
		_onUnknownMessageTypeCallback = std::bind(p_funct, std::placeholders::_1, std::placeholders::_2);
	}

	void Server::sendTo(const Server::EmiterID& p_emiterID, const spk::Network::Message& p_msg)
	{
		if (_clients.contains(p_emiterID) == false)
			spk::throwException(L"Emiter link to no client");
		_clients[p_emiterID].send(p_msg);
	}
}