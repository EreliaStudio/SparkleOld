#include "playground.hpp"

class Test : public spk::Widget::Interface
{
private:
	int count = 0;
	spk::WidgetComponent::Box _box;

	void _onGeometryChange()
	{
		_box.setGeometry(area(), 10);
		_box.setDepth(depth());
	}

	void _onRender()
	{		
		_box.render();
	}
	
	bool _onUpdate()
	{
		return (false);
	}

public:
	Test(const std::wstring &p_name) : spk::Widget::Interface(p_name)
	{

	}

	void setColors(spk::Color p_colorA, spk::Color p_colorB)
	{
		_box.setColors(p_colorA, p_colorB);
	}
	
	~Test()
	{

	}
};

int main()
{
    spk::Application app(L"Playground", 800);
    spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);

    // Create main widget
    std::shared_ptr<Test> mainWidget = app.addRootWidget<Test>(L"MainWidget");
    mainWidget->setColors(spk::Color(210, 25, 25, 255), spk::Color(180, 25, 25, 255));
    mainWidget->setGeometry(spk::Vector2Int(0, 0), app.size());
    mainWidget->activate();

	for (size_t i = 0; i < 4; i++)
	{
		std::shared_ptr<Test> parentWidget = mainWidget->addChildrenWidget<Test>(L"Widget Parent [" + std::to_wstring(i) + L"]");
		parentWidget->setColors(spk::Color(25, 25, 210, 255), spk::Color(25, 25, 180, 255));

		spk::Vector2Int anchor = spk::Vector2Int(app.size().x / 8 + (app.size().x / 2) * (i / 2), app.size().y / 8 + (app.size().y / 2) * (i % 2));
		spk::Vector2UInt size = spk::Vector2Int(app.size().x / 4, app.size().y / 4);

		parentWidget->setGeometry(anchor, size);
		parentWidget->activate();

		for (size_t j = 0; j < 4; j++)
		{
			std::shared_ptr<Test> childrenWidget = parentWidget->addChildrenWidget<Test>(L"Widget Children [" + std::to_wstring(i) + L"]");
			childrenWidget->setColors(spk::Color(25, 210, 25, 255), spk::Color(25, 180, 25, 255));
			
			spk::Vector2Int anchor = spk::Vector2Int(-static_cast<int>(parentWidget->size().x / 4) + parentWidget->size().x * (j / 2), -static_cast<int>(parentWidget->size().x / 4) + parentWidget->size().y * (j % 2));
			spk::Vector2UInt size = spk::Vector2UInt(parentWidget->size().x / 2, parentWidget->size().y / 2);

			childrenWidget->setGeometry(anchor, size);
			childrenWidget->activate();
		}
	}

    return (app.run());
}
