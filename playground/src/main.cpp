#include "playground.hpp"

class Test : public spk::Widget::Interface
{
private:
	spk::OpenGL::GraphicalDevice gdeviceA;

	size_t selectedStorage = 0;
	std::vector<spk::OpenGL::GraphicalDevice::Storage*> storages;

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
	std::vector<spk::Vector2> verticesDatas = {spk::Vector2(0.5f, 0.5f), spk::Vector2(-0.5f, 0.5f), spk::Vector2(0.5f, -0.5f), spk::Vector2(-0.5f, -0.5f)};
	std::vector<spk::Color> colors = {
		spk::Color(255, 0, 0),
		spk::Color(0, 255, 0),
		spk::Color(0, 0, 255),
		spk::Color(0, 255, 255),
		spk::Color(255, 255, 0),
		spk::Color(255, 0, 255)
	};

	std::vector<unsigned int> indexesData = {3, 2, 1, 1, 2, 0};

	for (size_t i = 0; i < storages.size(); i++)
	{
		std::vector<spk::Color> colorDatas = {spk::Color(0, 0, 0), colors[i % 6], colors[i % 6], spk::Color(255, 255, 255)};

		storages[i]->activate();
		storages[i]->datas()->push(verticesDatas, colorDatas);
		storages[i]->indexes()->push(indexesData);
	}
}

void Test::_onRender()
{
	if (selectedStorage >= storages.size())
		return ;
	gdeviceA.activate();

	storages[selectedStorage]->activate();
	//storages[selectedStorage]->uniform(L"model_origin")->push(spk::Vector2(0, 0));

	gdeviceA.launch(storages[selectedStorage]->nbIndexes());
	storages[selectedStorage]->deactivate();
}

bool Test::_onUpdate()
{
	if (spk::Keyboard::instance()->inputStatus(spk::Keyboard::A) == spk::InputState::Pressed)
	{
		selectedStorage = 0;
		selectedStorage %= 6;
	}
	if (spk::Keyboard::instance()->inputStatus(spk::Keyboard::Z) == spk::InputState::Pressed)
	{
		selectedStorage += 1;
		selectedStorage %= 6;
	}
	if (spk::Keyboard::instance()->inputStatus(spk::Keyboard::E) == spk::InputState::Pressed)
	{
		selectedStorage += 5;
		selectedStorage %= 6;
	}

	return (false);
}

Test::Test(const std::wstring& p_name) : 
	spk::Widget::Interface(p_name)
{
	gdeviceA.loadFromFile(L"colorShader.vert", L"colorShader.frag");

	for (size_t i = 0; i < 1; i++)
	{
		if (i == 0)
			storages.push_back(gdeviceA.storage());
		else
			storages.push_back(gdeviceA.storage()->copy());
	}
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