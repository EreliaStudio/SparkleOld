#pragma once

#include "threading/spk_thread_safe_queue.hpp"
#include "design_pattern/spk_singleton.hpp"
#include "application/modules/spk_abstract_module.hpp"
#include "spk_system_define.hpp"
#include "math/spk_vector2.hpp"

namespace spk
{
	class WindowModule : public IMessageConsumerModule
	{
	private:
		void _handleMessage(SystemMessage& event);

	public:
		WindowModule(spk::ThreadSafeQueue<SystemMessage> &queue, const std::wstring& p_title, const spk::Vector2Int& p_size, void *p_apiModule);

		void render();
		void clear();
	};
}