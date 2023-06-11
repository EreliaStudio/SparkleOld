#include "application/modules/spk_abstract_module.hpp"

namespace spk
{
	IMessageConsumerModule::IMessageConsumerModule(spk::ThreadSafeQueue<SystemMessage *> &queue):
		_queue(queue)
	{}

	void IMessageConsumerModule::update()
	{
		while (_queue.empty() == false)
		{
#ifdef _WIN32

#elif __linux__
			xcb_generic_event_t *event(_queue.pop_front());

			_handleMessage(event);

			free(event);
#endif

		}
	}
}