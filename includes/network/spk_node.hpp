#pragma once

#include "network/spk_message.hpp"

namespace spk
{
	class CentralNode;

	class Node
	{
	private:
		CentralNode* _centralNode;

	public:
		Node();

		void link(CentralNode* p_centralNode);

		virtual void send(const spk::Message& p_msg) = 0;

		virtual void receive(const spk::Message& p_msg);
	};
}