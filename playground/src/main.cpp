#include "playground.hpp"


class AbstractWidget : public spk::InherenceObject<AbstractWidget>, public spk::ActivableObject
{
	friend class WidgetModule;

private:
	virtual void _onRender() = 0;
	virtual bool _onUpdate() = 0;

	virtual void _onGeometryChange() = 0;

	std::wstring _name;
	float _geometryEdited;
	float _depth;
	spk::Vector2Int _anchor;
	spk::Vector2Int _size;
	
	virtual void _render()
	{
		if (isActive() == false)
			return ;
		if (_geometryEdited == true)
		{
			_onGeometryChange();
			_geometryEdited = false;
		}
			
		_onRender();
		
		for (size_t i(0); i < childrens().size(); ++i)
		{
			childrens()[i]->_render();
		}
	}

	virtual bool _update()
	{
		if (isActive() == false)
			return false;
		for (size_t i(0); i < childrens().size(); ++i)
			if (childrens()[i]->_update() == true)
				return (true);
		
		return (_onUpdate());
	}


public:
	AbstractWidget(std::wstring p_name) :
		_name(p_name)
	{

	}

	~AbstractWidget()
	{
		for (size_t i(0); i < childrens().size(); ++i)
		{
			delete childrens()[i];
		}	
	}

	template <typename TChildrenType, typename ... Args>
	TChildrenType* addChildrenWidget(Args&& ... p_args)
	{
		TChildrenType * result = new TChildrenType(std::forward<Args>(p_args)...);

		addChild(result);
		result->setDepth(depth() + 1);

		return (result);
	}

	void setGeometry(spk::Vector2Int p_anchor, spk::Vector2Int p_size)
	{
		_anchor = p_anchor;
		_size = p_size;
		
		_geometryEdited = true;
	}
	
	void place(spk::Vector2Int p_anchor)
	{
		_anchor = p_anchor;
		
		_geometryEdited = true;
	}
	
	void move(spk::Vector2Int p_anchor)
	{
		_anchor += p_anchor;
		
		_geometryEdited = true;
	}

	void resize(spk::Vector2Int p_size)
	{
		_size = p_size;
		
		_geometryEdited = true;
	}

	void sortChildrensByDepth()
	{
		std::sort(childrens().begin(), childrens().end(), [](AbstractWidget* lhs, AbstractWidget* rhs) {
					return lhs->depth() > rhs->depth();
				});
	}

	void setDepth(const float& p_depth)
	{
		float delta(p_depth - _depth);

		_depth = p_depth;

		for (size_t i(0); i < childrens().size(); ++i)
			childrens()[i]->setDepth(childrens()[i]->depth() + delta);
	}

	void rename(const std::wstring& p_name)
	{
		_name = p_name;
	}

	constexpr const std::wstring& name() const {return (_name);}
	constexpr const float& depth() const { return (_depth);	}

	bool isPointed(spk::Vector2Int p_point)
	{
		return (spk::Vector2Int::isInsideRectangle(p_point ,_anchor, _anchor + _size));
	}
};

class NoGraphicsWidget : public AbstractWidget
{
private:
	virtual void _onRender() {}

	virtual void _onGeometryChange() {}

	virtual void _render() { }
};

class OnlyGraphicsWidget : public AbstractWidget
{
private:
	virtual bool _onUpdate() { return (false); }
	virtual bool _update() { return (false); }
};

class WidgetModule : public spk::AbstractModule
{
private:
	class CentralWidget : public AbstractWidget
	{
		friend class WidgetModule;
		
	private:
		virtual void _onRender()
		{

		}
		virtual bool _onUpdate()
		{
			return (false);
		}

		virtual void _onGeometryChange()
		{

		}
	public:
		CentralWidget(std::wstring p_name) :
			AbstractWidget(p_name)
		{

		}
	};

	CentralWidget* _centralWidget;

public:
	WidgetModule() :
		_centralWidget(new CentralWidget(L"CentralWidget"))
	{
		_centralWidget->activate();
	}

	~WidgetModule()
	{
		delete _centralWidget;
	}
	
	void update()
	{
		_centralWidget->_update();
	}

	void render()
	{
	}

	AbstractWidget* centralWidget() const
	{
		return (_centralWidget);
	}
};


class MyApplication : public spk::AbstractApplication
{
private:
	spk::APIModule *_APIModule;
	spk::WindowModule *_windowModule;
	spk::MouseModule *_mouseModule;
	spk::KeyboardModule *_keyboardModule;

	WidgetModule* _widgetModule;

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

		addJob(L"Updater", [&]()
			   { _widgetModule->update(); });

		addJob(L"Updater", [&]()
			   { _mouseModule->updateMouse(); });
		addJob(L"Updater", [&]()
			   { _keyboardModule->updateKeyboard(); });

		addJob([&]()
			   { _windowModule->clear(); });
		addJob([&]()
			   { _widgetModule->render(); });
		addJob([&]()
			   { _windowModule->render(); });
	}

public:
	MyApplication(spk::Vector2Int p_size)
	{
		spk::Singleton<spk::Window>::instanciate(p_size);

		_APIModule = new spk::APIModule();
		_windowModule = new spk::WindowModule(_APIModule->windowQueue());
		_mouseModule = new spk::MouseModule(_APIModule->mouseQueue());
		_keyboardModule = new spk::KeyboardModule(_APIModule->keyboardQueue());

		_widgetModule = new WidgetModule();
		_widgetModule->centralWidget()->setGeometry(0, p_size);
	}

	~MyApplication()
	{
		delete _APIModule;
		delete _windowModule;
		delete _mouseModule;
		delete _keyboardModule;
		delete _widgetModule;
		
		spk::Singleton<spk::Window>::release();
		spk::Singleton<spk::Mouse>::release();
		spk::Singleton<spk::Keyboard>::release();
	}

	AbstractWidget* centralWidget()
	{
		return (_widgetModule->centralWidget());
	}

	void resize(spk::Vector2Int p_size)
	{
		spk::Singleton<spk::Window>::instance()->setGeometry(p_size);
		_widgetModule->centralWidget()->setGeometry(0, p_size);
	}
};

class WidgetA : public AbstractWidget
{
private:
	void _onRender()
	{
	}
	void _onGeometryChange()
	{
		
	}
	bool _onUpdate()
	{
		return (false);
	}

public:
	WidgetA(std::wstring p_name) :
		AbstractWidget(p_name)
	{

	}
};


class WidgetB : public AbstractWidget
{
private:
	void _onRender()
	{
	}
	void _onGeometryChange()
	{

	}
	bool _onUpdate()
	{
		return (false);
	}
public:
	WidgetB(std::wstring p_name) :
		AbstractWidget(p_name)
	{

	}
};


int main()
{
	MyApplication app({300, 600});

	app.resize({800, 600});

	WidgetA* tmpA = app.centralWidget()->addChildrenWidget<WidgetA>(L"WidgetA");
	WidgetA* tmpA2 = app.centralWidget()->addChildrenWidget<WidgetA>(L"WidgetA2");

	tmpA->setDepth(2);
	tmpA2->setDepth(20);

	tmpA->addChildrenWidget<WidgetB>(L"WidgetB")->activate();
	tmpA->addChildrenWidget<WidgetB>(L"WidgetB2")->activate();
	tmpA->addChildrenWidget<WidgetB>(L"WidgetB3")->activate();

	tmpA2->addChildrenWidget<WidgetB>(L"WidgetB")->activate();
	tmpA2->addChildrenWidget<WidgetB>(L"WidgetB2")->activate();

	tmpA->activate();
	tmpA2->activate();

	app.centralWidget()->sortChildrensByDepth();

	return app.run();
}