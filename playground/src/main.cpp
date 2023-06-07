#include "playground.hpp"
#include <xcb/xcb.h>

class Window : public spk::Singleton<Window>
{
	friend class spk::Singleton<Window>;

private:
	xcb_connection_t *_connection;
	xcb_screen_t *_screen;
	xcb_window_t _window;

	Window(size_t width, size_t height)
	{
		_connection = xcb_connect(NULL, NULL);
		if (xcb_connection_has_error(_connection))
			throw std::runtime_error("Cannot open connection to the X server");

		_screen = xcb_setup_roots_iterator(xcb_get_setup(_connection)).data;

		_window = xcb_generate_id(_connection);

		xcb_create_window(
			_connection,
			XCB_COPY_FROM_PARENT,
			_window,
			_screen->root,
			0, 0,
			width, height,
			10,
			XCB_WINDOW_CLASS_INPUT_OUTPUT,
			_screen->root_visual,
			0, NULL);

		xcb_map_window(_connection, _window);
	}

public:
	void render()
	{
		xcb_flush(_connection);
	}

	void clear()
	{
		xcb_clear_area(_connection, 0, _window, 0, 0, 800, 800);
	}
};

class AbstractModule
{
public:
	virtual void update() = 0;
};

class APIModule : public AbstractModule
{
private:
	spk::ThreadSafeQueue<int> _windowQueue;
	spk::ThreadSafeQueue<int> _mouseQueue;
	spk::ThreadSafeQueue<int> _keyboardQueue;

public:
	APIModule()
	{
	}

	~APIModule()
	{
	}

	void update()
	{
		spk::cout << "APIModule::update()" << std::endl;
	}

	void render()
	{
		spk::cout << "APIModule::render()" << std::endl;
	}

	spk::ThreadSafeQueue<int> &windowQueue() { return _windowQueue; }

	spk::ThreadSafeQueue<int> &mouseQueue() { return _mouseQueue; }

	spk::ThreadSafeQueue<int> &keyboardQueue() { return _keyboardQueue; }
};

class IMessageConsumerModule : public AbstractModule
{
private:
	spk::ThreadSafeQueue<int> &_queue;

public:
	IMessageConsumerModule(spk::ThreadSafeQueue<int> &queue) : _queue(queue)
	{
	}

	void update()
	{
	}
};

class WindowModule : public IMessageConsumerModule
{
public:
	WindowModule(spk::ThreadSafeQueue<int> &queue) : IMessageConsumerModule(queue)
	{
	}

	void render()
	{
		spk::Singleton<Window>::instance()->render();
	}

	void clear()
	{
		spk::Singleton<Window>::instance()->clear();
	}
};

class MouseModule : public IMessageConsumerModule
{
public:
	MouseModule(spk::ThreadSafeQueue<int> &queue) : IMessageConsumerModule(queue)
	{
	}
};

class KeyboardModule : public IMessageConsumerModule
{
public:
	KeyboardModule(spk::ThreadSafeQueue<int> &queue) : IMessageConsumerModule(queue)
	{
	}
};

class MyApplication : public spk::AbstractApplication
{
private:
	APIModule *_APIModule;
	WindowModule *_windowModule;
	MouseModule *_mouseModule;
	KeyboardModule *_keyboardModule;

protected:
	void setupJobs()
	{
		addJob(L"Updater", [&]() { _APIModule->update(); });
		addJob(L"Updater", [&]() { _windowModule->update(); });
		addJob(L"Updater", [&]() { _mouseModule->update(); });
		addJob(L"Updater", [&]() { _keyboardModule->update(); });

		addJob([&]() { _windowModule->clear(); });
		addJob([&]() { _windowModule->render(); });
	}

public:
	MyApplication(size_t width, size_t height)
	{
		spk::Singleton<Window>::instanciate(width, height);

		_APIModule = new APIModule();
		_windowModule = new WindowModule(_APIModule->windowQueue());
		_mouseModule = new MouseModule(_APIModule->mouseQueue());
		_keyboardModule = new KeyboardModule(_APIModule->keyboardQueue());
	}

	~MyApplication()
	{
		delete _APIModule;
		delete _windowModule;
		delete _mouseModule;
		delete _keyboardModule;

		spk::Singleton<Window>::release();
	}
};

int main()
{
	MyApplication app(300, 600);

	return app.run();
}