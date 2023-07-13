#pragma once

#include "network/spk_message.hpp"
#include "threading/spk_thread_safe_queue.hpp"
#include "network/spk_source.hpp"

namespace spk
{ 
	class Sink
	{
	public:
		using Emiter = long long;
		using Content = std::pair<Emiter, spk::Message>;

	private:
		spk::ThreadSafeQueue<Content> _messagesToTreat;

	public:
		void start(const size_t& p_port)
		{
			spk::cout << L"Starting server on port " << p_port << std::endl;
		}

		void stop()
		{

		}

		void bind(const spk::Source& p_source)
		{

		}

		void unbind()
		{

		}

		void receive(Emiter p_emiter, const spk::Message& p_msg)
		{
			_messagesToTreat.push_back(std::make_pair(p_emiter, p_msg));
		}

		spk::ThreadSafeQueue<Content>& messagesToTreat() 
		{
			return (_messagesToTreat);
		}
	};
}