#pragma once

#include "network/spk_message.hpp"
#include "network/spk_sink.hpp"

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

		void _treatMessage(const spk::Message& p_msg)
		{
			auto callbackIt = _onMessageReceptionCallbacks.find(p_msg.header().id());
			if(callbackIt != _onMessageReceptionCallbacks.end())
			{
				callbackIt->second(p_msg);
			}
			else
			{
				spk::throwException(L"Callback not defined for message id [" + std::to_wstring(p_msg.header().id()) + L"]");
			}
		}

	public:
		Client() :
			_socketContextWorker(L"Client socket")
		{
			_readingSocketDataContract = _socketContextWorker.addJob([&](){
				spk::Message newMessage;

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

		~Client()
		{
			_socketContextWorker.stop();
		}

		void connect(const std::wstring& p_serverAddress, const size_t& p_serverPort)
		{
			_socket.connect(p_serverAddress, p_serverPort);
		}

		void disconnect()
		{
			_socket.close();
		}

		void treatMessages()
		{
			if (_socket.isConnected() == true)
				_socketContextWorker.resume();

			while (_messagesToTreat.empty() == false)
			{
				_treatMessage(_messagesToTreat.pop_front());
			}
		}

		template <typename Funct, typename ... Args>
		void setOnMessageReceptionCallback(const spk::Message::Type& p_id, Funct&& p_funct, Args&& ... p_args)
		{
			if (_onMessageReceptionCallbacks.contains(p_id) == true)
				spk::throwException(L"Callback already define for message ID [" + std::to_wstring(p_id) + L"]");
			_onMessageReceptionCallbacks[p_id] = std::bind(std::forward<Funct>(p_funct), std::placeholders::_1, std::forward<Args>(p_args)...);
		}

		void send(const spk::Message& p_msg)
		{
			_socket.send(p_msg);
		}
	};
}