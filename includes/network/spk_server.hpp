#pragma once

#include <map>
#include "network/spk_message.hpp"
#include "network/spk_sink.hpp"
#include "network/spk_source.hpp"

namespace spk
{
	class Server
	{		
	public:
		using Emiter = spk::Sink::Emiter;
	public:
		Sink _sink;
		std::map<long long, Source> _sources;
		std::map<spk::Message::ID, std::function<void(const Sink::Emiter&, const spk::Message&)>> _onMessageReceptionCallbacks;

		void _treatMessage(const Sink::Emiter& p_emiter, const spk::Message& p_msg)
		{
			auto callbackIt = _onMessageReceptionCallbacks.find(p_msg.header().id());
			if(callbackIt != _onMessageReceptionCallbacks.end())
			{
				callbackIt->second(p_emiter, p_msg);
			}
			else
			{
				spk::throwException(L"Callback not defined for message id [" + std::to_wstring(p_msg.header().id()) + L"]");
			}
		}

	public:
		Server()
		{

		}

		~Server()
		{
			
		}

		void start(size_t p_serverPort)
		{
			_sink.start(p_serverPort);
		}

		void stop()
		{
			_sink.stop();
		}

		void treatMessages()
		{
			while (_sink.messagesToTreat().empty() == false)
			{
				spk::Sink::Content sinkContent = _sink.messagesToTreat().pop_front();
				_treatMessage(sinkContent.first, sinkContent.second);
			}
		}

		template <typename Funct, typename ... Args>
		void setOnMessageReceptionCallback(const spk::Message::ID& p_id, Funct&& p_funct, Args&& ... p_args)
		{
			if (_onMessageReceptionCallbacks.contains(p_id) == true)
				spk::throwException(L"Callback already define for message ID [" + std::to_wstring(p_id) + L"]");
			_onMessageReceptionCallbacks[p_id] = std::bind(std::forward<Funct>(p_funct), std::placeholders::_1, std::placeholders::_2, std::forward<Args>(p_args)...);
		}

		void sendTo(Source p_target, const spk::Message& p_msg)
		{
			p_target.send(p_msg);
		}

		void sendTo(std::vector<Source> p_targets, const spk::Message& p_msg)
		{
			for (size_t i = 0; i < p_targets.size(); i++)
			{
				p_targets[i].send(p_msg);
			}
		}

		void sendToAll(const spk::Message& p_msg, Source p_toIgnore)
		{
			for (auto& source : _sources)
			{
				if (source.second != p_toIgnore)
					source.second.send(p_msg);
			}
		}
	};
}