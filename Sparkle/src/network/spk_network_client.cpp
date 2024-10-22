#include "network/spk_network_client.hpp"

namespace spk::Network
{
	void Client::_treatMessage(const spk::Network::Message &p_msg)
	{
		auto callbackIt = _onMessageReceptionCallbacks.find(p_msg.header().type());
		if (callbackIt != _onMessageReceptionCallbacks.end())
		{
			callbackIt->second(p_msg);
		}
		else
		{
			_onUnknowMessageReception(p_msg);
		}
	}

	void Client::_receiveMessage()
	{
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		fd_set socketToRead;
		FD_ZERO(&socketToRead);
		FD_SET(_socket.fileDescriptor(), &socketToRead);

		int activity = ::select(static_cast<int>(_socket.fileDescriptor()) + 1, &socketToRead, nullptr, nullptr, &timeout);

		if (activity == Socket::SocketError)
		{
			spk::throwException(L"Error while receiving message inside server process [" + std::to_wstring(Socket::getLastError()) + L"]");
		}
		else if (activity == 0)
		{
			return ;
		}
		else
		{
			spk::Network::Message newMessage;

			if (_socket.isConnected() == true && 
				FD_ISSET(_socket.fileDescriptor(), &socketToRead))
			{
				Socket::ReadResult readStatus = _socket.receive(newMessage);

				switch (readStatus)
				{
					case Socket::ReadResult::Closed:
						spk::cout << "Require closing socket" << std::endl;
						_socket.close();
						break;
					case Socket::ReadResult::Success:
						_messagesToTreat.push_back(std::move(newMessage));
						break;
				}
			}
		}
	}

	Client::Client() : _socketContextWorker(L"Client socket", [&]()
			{
				_receiveMessage();
			})
	{
		_socketContextWorker.start();
	}

	Client::~Client()
	{
		_socketContextWorker.stop();
	}

	void Client::connect(const std::wstring &p_serverAddress, const size_t &p_serverPort)
	{
		_socket.connect(p_serverAddress, p_serverPort);
	}

	void Client::disconnect()
	{
		_socket.close();
	}

	void Client::treatMessages()
	{
		while (_messagesToTreat.empty() == false)
		{
			_treatMessage(_messagesToTreat.pop_front());
		}
	}
	
	void Client::setUnknowMessageReceptionCallback(std::function<void(const spk::Network::Message&)> p_funct)
	{
		_onUnknowMessageReception = std::bind(p_funct, std::placeholders::_1);
	}

	void Client::send(const spk::Network::Message &p_msg)
	{
		if (_socket.isConnected() == false)
			spk::throwException(L"Can't send a message throught a disconnected socket");
		_socket.send(p_msg);
	}
}