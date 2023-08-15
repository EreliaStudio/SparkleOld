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

	Client::Client() : _socketContextWorker(L"Client socket")
	{
		_readingSocketDataContract = _socketContextWorker.addJob(L"Reading message", [&]()
			{
				spk::Network::Message newMessage;

				if (_socket.isConnected() == true)
				{
					Socket::ReadResult readStatus = _socket.receive(newMessage);

					switch (readStatus)
					{
						case Socket::ReadResult::Closed:
							spk::cout << "Require closing socket" << std::endl;
							_socket.close();
							break;
						case Socket::ReadResult::Success:
							_messagesToTreat.push_back(newMessage);
							break;
					}
				}
			});
		_socketContextWorker.start();
		_socketContextWorker.pause();
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
		if (_socket.isConnected() == true)
			_socketContextWorker.resume();

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