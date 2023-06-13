#include "application/modules/spk_abstract_module.hpp"

namespace spk
{
	IMessageConsumerModule::IMessageConsumerModule(spk::ThreadSafeQueue<SystemMessage> &queue):
		_queue(queue)
	{}

	void IMessageConsumerModule::update()
	{
		while (_queue.empty() == false)
		{
			SystemMessage event(_queue.pop_front());

			_handleMessage(event);

		}
	}
}