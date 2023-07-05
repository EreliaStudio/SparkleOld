#include "playground.hpp"

struct Context
{
	int value;

	Context()
	{

	}
};

using RendererContext = spk::ContextManager<Context>::ReadOnlyAccessor;
using UpdaterContext = spk::ContextManager<Context>::ReadWriteAccessor;

int main()
{
	spk::ContextManager<Context>::instanciate();

	spk::cout << "1 ) ValueA (" << &RendererContext::get() << "): " << RendererContext::get().value << std::endl;
	spk::cout << "1 ) ValueB (" << &UpdaterContext::get() << "): " << UpdaterContext::get().value << std::endl;
	spk::cout << " -----" << std::endl;
	UpdaterContext::get().value = 10;

	spk::cout << "2 ) ValueA (" << &RendererContext::get() << "): " << RendererContext::get().value << std::endl;
	spk::cout << "2 ) ValueB (" << &UpdaterContext::get() << "): " << UpdaterContext::get().value << std::endl;
	spk::cout << " -----" << std::endl;

	UpdaterContext::swap();

	spk::cout << "3 ) ValueA (" << &RendererContext::get() << ") : " << RendererContext::get().value << std::endl;
	spk::cout << "3 ) ValueB (" << &UpdaterContext::get() << ") : " << UpdaterContext::get().value << std::endl;
	spk::cout << " -----" << std::endl;
	

	UpdaterContext::get().value = 20;

	spk::cout << "4 ) ValueA (" << &RendererContext::get() << ") : " << RendererContext::get().value << std::endl;
	spk::cout << "4 ) ValueB (" << &UpdaterContext::get() << ") : " << UpdaterContext::get().value << std::endl;
	spk::cout << " -----" << std::endl;
	

	UpdaterContext::swap();

	spk::cout << "5 ) ValueA (" << &RendererContext::get() << ") : " << RendererContext::get().value << std::endl;
	spk::cout << "5 ) ValueB (" << &UpdaterContext::get() << ") : " << UpdaterContext::get().value << std::endl;
	spk::cout << " -----" << std::endl;
	
	return 0;
}
