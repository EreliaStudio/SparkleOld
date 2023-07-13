#pragma once

#include "network/spk_message.hpp"
#include "threading/spk_thread_safe_queue.hpp"

namespace spk
{ 
	class Source
	{
	private:
		struct DestinationAddress
		{
			std::wstring address;
			std::wstring port;

			DestinationAddress()
			{

			}
			void resolve(const std::wstring& p_serverAddress, const size_t& p_serverPort)
			{
				address = p_serverAddress;
				port = std::to_wstring(p_serverPort);
			}
		};

		DestinationAddress _destinationAddress;

		long long _id;
		spk::ThreadSafeQueue<spk::Message>  _messagesToSend;

		void _launchConnection()
		{
			spk::cout << L"Connecting source to " << _destinationAddress.address << L" with port " << _destinationAddress.port << std::endl;
		}

		void _stopConnection()
		{
			
		}

	public:
		void setID(const long long& p_id)
		{
			_id = p_id;
		}

		void send(const spk::Message& p_msg)
		{
			_messagesToSend.push_back(p_msg);
		}

		spk::ThreadSafeQueue<spk::Message>& messagesToSend() 
		{
			return (_messagesToSend);
		}
		
		bool operator == (const Source& p_other) const 
		{
			if (_id == p_other._id)
				return (true);
			return (false);
		}
		bool operator != (const Source& p_other) const 
		{
			if (_id != p_other._id)
				return (true);
			return (false);
		}

		void connect(const std::wstring& p_serverAddress, const size_t& p_serverPort)
		{
			_destinationAddress.resolve(p_serverAddress, p_serverPort);
			_launchConnection();
		}

		void disconnect()
		{
			_stopConnection();
		}

		void reconnect()
		{
			_launchConnection();
		}
	};
}