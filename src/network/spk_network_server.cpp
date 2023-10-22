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
		FD_ZERO(&_readingFDs);

		_socketListeningContract = _socketContextWorker.addJob(L"Accepting new connection", [&]()
			{
				Socket newSocket;
				if (_Acceptor.accept(newSocket) == true)
				{
					FD_SET(newSocket.fileDescriptor(), &_readingFDs);
					
					if (_maxFDs == Socket::SocketError || newSocket.fileDescriptor() > _maxFDs)
					{
						_maxFDs = newSocket.fileDescriptor();
					}

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
				if (_maxFDs == -1)
					return ;
				
				struct timeval timeout;
				timeout.tv_sec = 1;
				timeout.tv_usec = 0;

				fd_set socketToRead = _readingFDs;

				int activity = ::select(static_cast<int>(_maxFDs) + 1, &socketToRead, nullptr, nullptr, &timeout);

				if (activity == Socket::SocketError)
				{
					spk::throwException(L"Error while receiving message inside server process [" + std::to_wstring(getLastSocketErrorValue()) + L"]");
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
						if (it->second.isConnected() == true && 
							FD_ISSET(it->second.fileDescriptor(), &socketToRead))
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