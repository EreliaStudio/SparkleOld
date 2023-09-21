#include "sparkle.hpp"

class TestPipeline : public spk::GraphicalAPI::AbstractPipeline
{
private:
	void _loadProgram(
			const std::string& p_vertexName, const std::string& p_vertexCode,
			const std::string& p_fragmentName, const std::string& p_fragmentCode)
	{

	}

	void _pushStorageData(const void* p_data, const size_t& p_dataSize)
	{

	}

	void _renderObject(Object* p_object)
	{

	}

	void _activateObject(Object* p_object)
	{

	}
	void _deactivateObject(Object* p_object)
	{

	}

public: 
	TestPipeline(const std::filesystem::path& p_vertexShaderPath, const std::filesystem::path& p_fragmentShaderPath) :
		spk::GraphicalAPI::AbstractPipeline()
	{
		loadFromFile(p_vertexShaderPath, p_fragmentShaderPath);
	}
};

class Test : public spk::Widget::Interface
{
private:
	TestPipeline _pipeline;

	void _onGeometryChange()
	{
		
	}

	void _onRender()
	{

	}
	
	bool _onUpdate()
	{
		return (false);
	}

public:
	Test(const std::wstring& p_name) :
		spk::Widget::Interface(p_name),
		_pipeline(L"colorShader.vert", L"colorShader.frag")
	{

	}
	~Test()
	{

	}
};

int main()
{
	spk::Application app(L"Coucou", 400);
	spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Qwerty);

	std::shared_ptr<Test> test = app.addRootWidget<Test>(L"Test");
	test->setGeometry(spk::Vector2Int(0, 0), spk::Vector2UInt(400, 400));
	test->activate();

	return (app.run());
};