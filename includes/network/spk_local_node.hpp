#pragma once

#include <map>
#include <functional>
#include "network/spk_message.hpp"
#include "network/spk_node.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	class LocalNode : public spk::Node
	{
	private:
		std::map<spk::Message::Type, std::function<void(const spk::Message&)>> _onMessageReceptionCallbacks;

		std::function<void(const spk::Message&)> _onUnknowMessageReception = [&](const spk::Message& p_msg){
			spk::throwException(L"Message [" + std::to_wstring(p_msg.header().id()) + L"] not defined in local node");
		};

	public:
		LocalNode();

		template <typename Funct, typename ... Args>
		void setOnMessageReceptionCallback(const spk::Message::Type& p_id, Funct&& p_funct, Args&& ... p_args)
		{
			if (_onMessageReceptionCallbacks.contains(p_id) == true)
				spk::throwException(L"Callback already defined for message type [" + std::to_wstring(p_id) + L"]");
			_onMessageReceptionCallbacks[p_id] = std::bind(std::forward<Funct>(p_funct), std::placeholders::_1, std::forward<Args>(p_args)...);
		}

		template <typename Funct, typename ... Args>
		void setUnknowMessageReceptionCallback(Funct&& p_funct, Args&& ... p_args)
		{
			_onUnknowMessageReception = std::bind(std::forward<Funct>(p_funct), std::placeholders::_1, std::forward<Args>(p_args)...);
		}

		void send(const spk::Message& p_msg);
	};
}