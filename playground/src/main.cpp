#include "playground.hpp"

class MyApplication : public spk::AbstractApplication
{
private:
	spk::APIModule *_APIModule;
	spk::WindowModule *_windowModule;
	spk::MouseModule *_mouseModule;
	spk::KeyboardModule *_keyboardModule;

protected:
	void setupJobs()
	{
		addJob([&]()
			   { _APIModule->update(); });
		addJob(L"Updater", [&]()
			   { _windowModule->update(); });
		addJob(L"Updater", [&]()
			   { _mouseModule->update(); });
		addJob(L"Updater", [&]()
			   { _keyboardModule->update(); });

		addJob([&]()
			   { _windowModule->clear(); });
		addJob([&]()
			   { _windowModule->render(); });
	}

public:
	MyApplication(size_t width, size_t height)
	{
		spk::Singleton<spk::Window>::instanciate(width, height);

		_APIModule = new spk::APIModule();
		_windowModule = new spk::WindowModule(_APIModule->windowQueue());
		_mouseModule = new spk::MouseModule(_APIModule->mouseQueue());
		_keyboardModule = new spk::KeyboardModule(_APIModule->keyboardQueue());
	}

	~MyApplication()
	{
		delete _APIModule;
		delete _windowModule;
		delete _mouseModule;
		delete _keyboardModule;

		spk::Singleton<spk::Window>::release();
	}
};

int main()
{
	MyApplication app(300, 600);

	return app.run();
}