#include "sparkle.hpp"

// class Test : public spk::Widget::Interface
// {
// private:
// 	spk::GraphicalDevice gdeviceA;

// 	spk::GraphicalDevice::Object _object;

// 	float rotationValue(0);

// 	void _onGeometryChange();
// 	void _onRender();
	
// 	bool _onUpdate();

// public:
// 	Test(const std::wstring& p_name);
// 	~Test();
// };

// void Test::_onGeometryChange()
// {
// }

// void Test::_onRender()
// {
// 	_object.render();
// }

// bool Test::_onUpdate()
// {
// 	rotationValue += 0.1f * TimeMetrics.delataTime();
	
// 	gdeviceA.setContext("rotation", rotationValue); 
// 	return (false);
// }

// Test::Test(const std::wstring& p_name) : 
// 	spk::Widget::Interface(p_name)
// {
// 	gdeviceA.loadFromFile(L"colorShader.vert", L"colorShader.frag");

// 	_object = gdeviceA.createObject();
// 	_object.setStorageSize(4);
	
// 	std::vector<Vector2> vertices;
// 	std::vector<Vector3> colors;

// 	_object.setStorage("model", vertices);
// 	_object.setStorage("color", colors);

// 	_object.setUniform("position", Vector2(0.f, 0.2f));
// }

// Test::~Test()
// {

// }


int main()
{
	spk::Application app(L"Coucou", 400);
	// spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);

	// // Test* test = app.addRootWidget<Test>(L"Test");
	// // test->setGeometry(spk::Vector2Int(0, 0), spk::Vector2UInt(400, 400));
	// // test->activate();

	return (app.run());
};