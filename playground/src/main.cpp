#include "playground.hpp"


class MyApplication : public spk::AbstractApplication
{
private:
	spk::APIModule *_APIModule;
	spk::TimeModule *_timeModule;
	spk::WindowModule *_windowModule;
	spk::MouseModule *_mouseModule;
	spk::KeyboardModule *_keyboardModule;

	spk::WidgetModule* _widgetModule;

protected:
	void setupJobs()
	{
		addJob([&]()
			   { _APIModule->update(); });

		addJob(L"Updater", [&]()
			   { _timeModule->update(); });

		addJob(L"Updater", [&]()
			   { _windowModule->update(); });

		addJob(L"Updater", [&]()
			   { _mouseModule->update(); });
		addJob(L"Updater", [&]()
			   { _keyboardModule->update(); });

		addJob(L"Updater", [&]()
			   { _widgetModule->update(); });

		addJob(L"Updater", [&]()
			   { _mouseModule->updateMouse(); });
		addJob(L"Updater", [&]()
			   { _keyboardModule->updateKeyboard(); });

	// Assign a custom value
    valueFromDefault = 100;

    // Test callback subscription and triggering
    bool callbackTriggered = false;
    auto callback = [&callbackTriggered]() { callbackTriggered = true; };

public:
	MyApplication(spk::Vector2Int p_size)
	{
		_APIModule = new spk::APIModule();
		_timeModule = new spk::TimeModule();
		_windowModule = new spk::WindowModule(_APIModule->windowQueue(), p_size);
		_mouseModule = new spk::MouseModule(_APIModule->mouseQueue());
		_keyboardModule = new spk::KeyboardModule(_APIModule->keyboardQueue());


    // Change default value while on custom should not trigger callback
    defaultValue = 80;
    assert(callbackTriggered == false);

    // Change value to trigger callback
    valueFromDefault = 50;
    assert(callbackTriggered == true);

	~MyApplication()
	{
		delete _APIModule;
		delete _timeModule;
		delete _windowModule;
		delete _mouseModule;
		delete _keyboardModule;

		delete _widgetModule;
	}

	spk::AbstractWidget* centralWidget()
	{
		return (_widgetModule->centralWidget());
	}

	void resize(spk::Vector2Int p_size)
	{
		spk::Singleton<spk::Window>::instance()->setGeometry(p_size);
		_widgetModule->centralWidget()->setGeometry(0, p_size);
	}
};


int main()
{
	MyApplication app({300, 600});

	return app.run();
}