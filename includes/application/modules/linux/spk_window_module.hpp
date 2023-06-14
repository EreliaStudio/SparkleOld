#pragma once

#include "application/modules/spk_abstract_module.hpp"
#include "math/spk_vector2.hpp"

namespace spk
{
	class WindowModule : public IMessageConsumerModule
	{
	private:
		void _handleMessage(SystemMessage& p_event);

	public:
		WindowModule(spk::ThreadSafeQueue<SystemMessage> &p_queue, const std::wstring& p_title, const spk::Vector2Int& p_size);

		void render();
		void clear();
	};
}