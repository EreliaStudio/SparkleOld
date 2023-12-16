#include "playground.hpp"

class PseudoApplication;

class Viewport
{
private:
    spk::Vector2Int _anchor;
    spk::Vector2UInt _size;

public:
    Viewport() :
        _anchor(0, 0),
        _size(0, 0)
    {

    }

    void setGeometry(const spk::Vector2Int& p_anchor, const spk::Vector2UInt& p_size)
    {
        _anchor = p_anchor;
        _size = p_size;
    }

    const spk::Vector2Int& anchor() const 
    {
        return (_anchor);
    }

    const spk::Vector2Int& size() const 
    {
        return (_size);
    }

    void use() const
    {

    }
};

class Widget
{
    friend class WidgetModule;

private:
    bool _isActive;
    Viewport _viewport;
    bool _needGeometryChange = false;
    Widget* _parent = nullptr;
    std::vector<Widget*> _childrens;

    void _render()
    {
        if (isActive() == false)
            return ;

        if (_parent != nullptr)
            _parent->_activateViewport();

        if (_needGeometryChange == true)
        {
            _onGeometryChange();
            _needGeometryChange = false;
        }

        _onRender();

        for (size_t i = 0; i < _childrens.size(); i++)
        {
            spk::cout << " - ";
            _childrens[i]->_render();
        }
    }

    bool _update()
    {
        if (isActive() == false)
            return (false);

        for (size_t i = 0; i < _childrens.size(); i++)
        {
            if (_childrens[i]->_update() == true)
            {
                return (true);
            }
            spk::cout << " - ";
        }

        return (_onUpdate());
    }

    virtual void _onRender() = 0;
    virtual bool _onUpdate() = 0;
    virtual void _onGeometryChange() = 0;

    void _activateViewport() const
    {
        _viewport.use();
    }

    void _removeChild(Widget* p_child)
    {
        auto it = std::remove(_childrens.begin(), _childrens.end(), p_child);
        _childrens.erase(it, _childrens.end());

        p_child->_parent = nullptr;
    }

public:
    Widget();
    ~Widget();

    Widget(const Widget& p_other) = delete;
    Widget& operator=(const Widget& p_other) = delete;

    Widget(Widget&& p_other) noexcept :
        _isActive(p_other._isActive), 
        _viewport(std::move(p_other._viewport)), 
        _needGeometryChange(true), 
        _parent(p_other._parent)
    {

    }

    Widget& operator=(const Widget&& p_other)
    {
        if (this != &p_other)
        {
            _isActive = p_other._isActive;
            _viewport = std::move(p_other._viewport);
            _needGeometryChange = true;
            _parent = p_other._parent;
        }
        return *this;
    }

    void addChild(Widget* p_child)
    {
        if (p_child->_parent != nullptr)
            p_child->_parent->_removeChild(p_child);
        _childrens.push_back(p_child);
        p_child->_parent = this;
    }

    void setGeometry(const spk::Vector2Int& p_anchor, const spk::Vector2UInt& p_size)
    {
        _viewport.setGeometry(p_anchor, p_size);
        _needGeometryChange = true;
    }

    const Widget* parent() const
    {
        return (_parent);
    }

    const spk::Vector2Int& anchor() const 
    {
        return (_viewport.anchor());
    }

    const spk::Vector2Int& size() const 
    {
        return (_viewport.size());
    }

    void setActive(bool p_state)
    {
        _isActive = p_state;
    }

    bool isActive() const
    {
        return (_isActive);
    }
};

class WidgetModule
{
public:
    class CentralWidget : public Widget
    {
    private:
        void _onGeometryChange()
        {

        }

        void _onRender()
        {
            spk::cout << "Central widget";
        }

        bool _onUpdate()
        {
            spk::cout << "Central widget";
            return (false);
        }

    public:
        CentralWidget()
        {
            DEBUG_LINE();
        }

        ~CentralWidget()
        {
            DEBUG_LINE();
        }
    };

private:
    CentralWidget _centralWidget;

public:
    WidgetModule()
    {
        _centralWidget.setActive(true);
    }

    CentralWidget* centralWidget()
    {
        return (&_centralWidget);
    }

    void renderWidgets()
    {
        _centralWidget._render();
    }

    void updateWidgets()
    {
        _centralWidget._update();
    }
};

class PseudoApplication
{
    friend class Widget;

private:
    static inline PseudoApplication* _instance;

    size_t _index = 0;

    WidgetModule _widgetModule;

    void _executeRenderTick()
    {
        spk::cout << "Render tick : ";
        _widgetModule.renderWidgets();
        spk::cout << std::endl;
    }

    void _executeUpdateTick()
    {
        spk::cout << "Update tick : ";
        _widgetModule.updateWidgets();
        spk::cout << std::endl;
    }

    WidgetModule::CentralWidget* _centralWidget()
    {
        return (_widgetModule.centralWidget());
    }

public:
    PseudoApplication()
    {
        _instance = this;
    }
    ~PseudoApplication()
    {
        DEBUG_LINE();
        if (_instance == this)
        {
            DEBUG_LINE();   
            _instance = nullptr;
        }
    }

    static PseudoApplication* instance() {return (_instance);}

    const size_t& index() const
    {
        return (_index);
    }

    int run()
    {
        for (size_t i = 0; i < 2; i++)
        {
            spk::cout << "Frame : [" << i << "]" << std::endl;
            _index = i;
            _executeRenderTick();
            _executeUpdateTick();
        }
        return (0);
    }
};


Widget::Widget() : 
    _isActive(false),
    _parent(nullptr)
{
    DEBUG_LINE();
    if (PseudoApplication::instance() != nullptr && PseudoApplication::instance()->_centralWidget() != this)
        PseudoApplication::instance()->_centralWidget()->addChild(this);
}

Widget::~Widget()
{
    DEBUG_LINE();
    if (_parent != nullptr)
    {
        _parent->_removeChild(this);
    }
    for (size_t i = 0; i < _childrens.size(); i++)
    {
        _childrens[i]->_parent = PseudoApplication::instance()->_centralWidget();
    }
}

class WidgetA : public Widget
{
private:
    void _onGeometryChange()
    {

    }

    void _onRender()
    {
        spk::cout << __CLASS__;
    }

    bool _onUpdate()
    {
        spk::cout << __CLASS__;
        return (false);
    }

public:
    WidgetA()
    {
        DEBUG_LINE();
    }

    ~WidgetA()
    {
        DEBUG_LINE();
    }
};

class WidgetB : public Widget
{
private:
    void _onGeometryChange()
    {

    }
    
    void _onRender()
    {
        spk::cout << __CLASS__;
    }

    bool _onUpdate()
    {
        spk::cout << __CLASS__;
        return (false);
    }

public:
    WidgetB()
    {
        DEBUG_LINE();
    }

    ~WidgetB()
    {
        DEBUG_LINE();
    }
};

class WidgetC : public Widget
{
private:
    WidgetA _widgetA;

    void _onGeometryChange()
    {

    }
    
    void _onRender()
    {
        spk::cout << __CLASS__;
    }

    bool _onUpdate()
    {
        spk::cout << __CLASS__;
        return (false);
    }

public:
    WidgetC()
    {
        DEBUG_LINE();
        addChild(&_widgetA);
        _widgetA.setActive(true);
    }

    ~WidgetC()
    {
        DEBUG_LINE();
    }
};

void runA(PseudoApplication& p_app)
{
    spk::cout << "--- Pre Run A ---" << std::endl;
    
    WidgetB widgetB3;
    widgetB3.setGeometry(spk::Vector2Int(0, 0), spk::Vector2UInt(200, 200));
    widgetB3.setActive(true);

    spk::cout << "--- Run A ---" << std::endl;

    p_app.run();

    spk::cout << "--- Post Run A ---" << std::endl;
}

void runB(PseudoApplication& p_app, WidgetC& widgetC1)
{
    spk::cout << "--- Pre Run B ---" << std::endl;

    WidgetA widgetA;
    widgetA.setGeometry(spk::Vector2Int(100, 100), spk::Vector2UInt(100, 100));
    widgetA.setActive(true);
    widgetC1.addChild(&widgetA);
    
    spk::cout << "--- Run B ---" << std::endl;

    p_app.run();

    spk::cout << "--- Post Run B ---" << std::endl;
}

void runC(PseudoApplication& p_app)
{
    spk::cout << "--- Pre Run C ---" << std::endl;

    spk::cout << "--- Run C ---" << std::endl;
    p_app.run();
    spk::cout << "--- Post Run C ---" << std::endl;
}

void scopedMain()
{
    spk::cout << "--- Program initialization ---" << std::endl;
    PseudoApplication app = PseudoApplication();

    WidgetB widgetB1;
    widgetB1.setGeometry(spk::Vector2Int(0, 0), spk::Vector2UInt(200, 200));
    widgetB1.setActive(true);

    WidgetB widgetB2;
    widgetB2.setGeometry(spk::Vector2Int(200, 0), spk::Vector2UInt(200, 200));
    widgetB2.setActive(true);

    WidgetC widgetC1;
    widgetC1.setGeometry(spk::Vector2Int(200, 200), spk::Vector2UInt(400, 400));
    widgetC1.setActive(true);

    runA(app);

    runB(app, widgetC1);

    runC(app);
    spk::cout << "--- Program closure ---" << std::endl;
}

int main()
{
    scopedMain();

    spk::cout << " -------- " << std::endl;

    return (0);
}
