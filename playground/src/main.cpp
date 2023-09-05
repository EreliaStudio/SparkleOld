#include "playground.hpp"

class Test : public spk::Widget::Interface
{
private:
	spk::OpenGL::GraphicalDevice gdeviceA;

	int selectedStorage = 1;
	spk::OpenGL::GraphicalDevice::Storage* dataStorages[3];
	spk::OpenGL::GraphicalDevice::Storage* indexesStorage;

	bool baked = false;

	void _onGeometryChange();
	void _onRender();
	
	bool _onUpdate();

public:
	Test(const std::wstring& p_name);
	~Test();
};

void Test::_onGeometryChange()
{
	std::vector<spk::Vector2> verticesDatas[3] = {
		{spk::Vector2(0.5f, 0.5f), spk::Vector2(-0.5f, 0.5f), spk::Vector2(0.5f, -0.5f), spk::Vector2(-0.5f, -0.5f)},
		{spk::Vector2(0.5f, 0.5f), spk::Vector2(-0.5f, 0.5f), spk::Vector2(0.5f, -0.5f), spk::Vector2(-0.5f, -0.5f)},
		{spk::Vector2(0.5f, 0.5f), spk::Vector2(-0.5f, 0.5f), spk::Vector2(0.5f, -0.5f), spk::Vector2(-0.5f, -0.5f)}
	};
	std::vector<spk::Color> colorDatas[3] = {
		{spk::Color(255, 255, 255), spk::Color(255, 0, 0), spk::Color(255, 0, 0), spk::Color(0, 0, 0)},
		{spk::Color(255, 255, 255), spk::Color(0, 255, 0), spk::Color(0, 255, 0), spk::Color(0, 0, 0)},
		{spk::Color(255, 255, 255), spk::Color(0, 0, 255), spk::Color(0, 0, 255), spk::Color(0, 0, 0)}
	};

	std::vector<unsigned int> indexesData = {3, 2, 1, 1, 2, 0};

	for (size_t i = 0; i < 3; i++)
	{
		dataStorages[i]->push(verticesDatas[i], colorDatas[i]);
	}
	indexesStorage->push(indexesData);
}

void Test::_onRender()
{
	gdeviceA.activate();

	dataStorages[selectedStorage]->activate();
	indexesStorage->activate();

	gdeviceA.launch(indexesStorage->nbElement());

	dataStorages[selectedStorage]->deactivate();
	indexesStorage->deactivate();
}

bool Test::_onUpdate()
{
	if (spk::Keyboard::instance()->inputStatus(spk::Keyboard::A) == spk::InputState::Pressed)
	{
		selectedStorage = 0;
	}
	if (spk::Keyboard::instance()->inputStatus(spk::Keyboard::Z) == spk::InputState::Pressed)
	{
		selectedStorage = 1;
	}
	if (spk::Keyboard::instance()->inputStatus(spk::Keyboard::E) == spk::InputState::Pressed)
	{
		selectedStorage = 2;
	}

	return (false);
}

Test::Test(const std::wstring& p_name) : 
	spk::Widget::Interface(p_name)
{
	gdeviceA.loadFromFile(L"colorShader.vert", L"colorShader.frag");

	for (size_t i = 0; i < 3; i++)
	{
		dataStorages[i] = gdeviceA.dataStorage()->copy();
	}
	indexesStorage = gdeviceA.indexesStorage()->copy();
}

Test::~Test()
{

}


int main()
{
	spk::Application app(L"Coucou", 400, spk::GraphicalAPI::OpenGL);
	spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);

	Test* test = app.addRootWidget<Test>(L"Test");
	test->setGeometry(spk::Vector2Int(0, 0), spk::Vector2UInt(400, 400));
	test->activate();

	return (app.run());
};