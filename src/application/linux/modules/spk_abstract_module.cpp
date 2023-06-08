#include "application/linux/modules/spk_abstract_module.hpp"

namespace spk
{
	IMessageConsumerModule::IMessageConsumerModule(spk::ThreadSafeQueue<xcb_generic_event_t *> &queue):
		_queue(queue)
	{}

	void IMessageConsumerModule::update()
	{
		while (_queue.empty() == false)
		{
			xcb_generic_event_t *event(_queue.pop_front());

			_handleMessage(event);

			free(event);
		}
	}
}