#include "sparkle.hpp"

class OpenGLPipeline : public spk::GraphicalAPI::AbstractPipeline
{
private:
	class OpenGLObject : public spk::GraphicalAPI::AbstractPipeline::Object
	{
	public:
		OpenGLObject(spk::GraphicalAPI::AbstractPipeline* p_owner, const spk::GraphicalAPI::AbstractPipeline::Object::Storage::Configuration& p_storageConfiguration) :
			spk::GraphicalAPI::AbstractPipeline::Object(p_owner, p_storageConfiguration)
		{

		}

		void push()
		{
			DEBUG_LINE();
		}

		void render()
		{
			DEBUG_LINE();
		}
	};

	void _loadProgram(
			const std::string& p_vertexName, const std::string& p_vertexCode,
			const std::string& p_fragmentName, const std::string& p_fragmentCode)
	{

	}

public: 
	OpenGLPipeline(const std::filesystem::path& p_vertexShaderPath, const std::filesystem::path& p_fragmentShaderPath) :
		spk::GraphicalAPI::AbstractPipeline()
	{
		loadFromFile(p_vertexShaderPath, p_fragmentShaderPath);
	}
	
	std::shared_ptr<Object> createObject()
	{
		std::shared_ptr<OpenGLObject> result = std::make_shared<OpenGLObject>(this, _storageConfiguration);

		return (result);
	}
};

class Test : public spk::Widget::Interface
{
private:
	OpenGLPipeline _pipeline;
	std::shared_ptr<OpenGLPipeline::Object> _object;

	void _onGeometryChange()
	{		
		/*
			Fill the object
		*/

		_object->push();
	}

	void _onRender()
	{
		_object->render();
	}
	
	bool _onUpdate()
	{
		return (false);
	}

public:
	Test(const std::wstring& p_name) :
		spk::Widget::Interface(p_name),
		_pipeline(L"colorShader.vert", L"colorShader.frag"),
		_object(_pipeline.createObject())
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