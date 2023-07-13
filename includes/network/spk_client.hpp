#pragma once

#include "network/spk_message.hpp"
#include "network/spk_sink.hpp"
#include "network/spk_source.hpp"

namespace spk
{
	class Client
	{
	public:
		spk::Sink _sink;
		spk::Source _source;

		std::map<spk::Message::ID, std::function<void(const spk::Message&)>> _onMessageReceptionCallbacks;

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
		Client()
		{

		}

		~Client()
		{
			
		}

		void connect(const std::wstring& p_serverAddress, const size_t& p_serverPort)
		{
			_source.connect(p_serverAddress, p_serverPort);
			_sink.bind(_source);
		}

		void disconnect()
		{
			_source.disconnect();
			_sink.unbind();
		}

		void reconnect()
		{
			_source.reconnect();
			_sink.bind(_source);
		}

		void treatMessages()
		{
			while (_sink.messagesToTreat().empty() == false)
			{
				spk::Sink::Content sinkContent = _sink.messagesToTreat().pop_front();
				_treatMessage(sinkContent.second);
			}
		}

		template <typename Funct, typename ... Args>
		void setOnMessageReceptionCallback(const spk::Message::ID& p_id, Funct&& p_funct, Args&& ... p_args)
		{
			_onMessageReceptionCallbacks[p_id] = std::bind(std::forward<Funct>(p_funct), std::placeholders::_1, std::forward<Args>(p_args)...);
		}

		void send(const spk::Message& p_msg)
		{
			_source.send(p_msg);
		}
	};
}