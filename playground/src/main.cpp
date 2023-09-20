#include "sparkle.hpp"

// template<typename T, typename... Rest>
// struct UnitImpl : public UnitImpl<Rest...>
// {
//     UnitImpl(const T& p_value, const Rest&... p_rest)
//         : UnitImpl<Rest...>(p_rest...), value(p_value)
//     {
//     }

//     T value;
// };

// template<typename T>
// struct UnitImpl<T>
// {
//     UnitImpl(const T& p_value) : value(p_value)
//     {
//     }

//     T value;
// };

// template<typename... Types>
// class Unit : public UnitImpl<Types...>
// {
// public:
//     Unit(const Types&... p_args) : UnitImpl<Types...>(p_args...)
//     {
//     }
// };

// class Test : public spk::Widget::Interface
// {
// private:
// 	spk::GraphicalDevice _device;
// 	spk::GraphicalDevice::Object _object;

// 	void _onGeometryChange()
// 	{
		
// 	}

// 	void _onRender()
// 	{
// 		_object.render();
// 	}
	
// 	bool _onUpdate()
// 	{
// 		return (false);
// 	}

// public:
// 	Test(const std::wstring& p_name) :
// 		spk::Widget::Interface(p_name),
// 		_device(L"ColorShaderVert.vert", L"ColorShaderFrag.frag"),
// 		_object(_device.createObject())
// 	{
// 		std::vector<spk::GraphicalDevice::Object::Unit<spk::Vector2, spk::Color> > units = {
// 			{ spk::Vector2(-0.5f, -0.5f), spk::Color(1.0f, 0.0f, 0.0f, 1.0f) },
// 			{ spk::Vector2(0.5f, -0.5f), spk::Color(0.0f, 1.0f, 0.0f, 1.0f) },
// 			{ spk::Vector2(0.5f, 0.5f), spk::Color(0.0f, 0.0f, 1.0f, 1.0f) },
// 			{ spk::Vector2(-0.5f, 0.5f), spk::Color(1.0f, 1.0f, 1.0f, 1.0f) }
// 		};

// 		_object->setModel(units.data(), units.size());
// 		_object.setAttribute("position", spk::Vector2(0.0f, 0.2f));
// 	}
// 	~Test()
// 	{

// 	}
// };

int main()
{
	spk::Application app(L"Coucou", 400);
	spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Qwerty);

	spk::GraphicalAPI::GraphicalDevice test;

	test.test();

	// Test* test = app.addRootWidget<Test>(L"Test");
	// test->setGeometry(spk::Vector2Int(0, 0), spk::Vector2UInt(400, 400));
	// test->activate();

	return (app.run());
};