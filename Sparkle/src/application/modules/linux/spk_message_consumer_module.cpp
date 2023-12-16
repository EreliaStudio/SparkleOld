#include "application/modules/spk_message_consumer_module.hpp"

namespace spk
{
	IMessageConsumerModule::IMessageConsumerModule(spk::ThreadSafeQueue<SystemMessage> &p_queue):
		_queue(p_queue)
	{}

	void IMessageConsumerModule::treatMessage()
	{
		while (_queue.empty() == false)
		{
			SystemMessage event(_queue.pop_front());

			_handleMessage(event);

			free(event);
		}
	}
}