#include "sparkle.hpp"
#include "pipeline.hpp"

#include <regex>

class Pipeline : public AbstractPipeline
{
private:
	void _loadProgram(
		const Configuration &p_configuration,
		const ShaderModule &p_vertexInput,
		const ShaderModule &p_fragmentInput) override
	{
	}

public:
	Pipeline(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput)
	{
		_loadPipeline(p_vertexInput, p_fragmentInput);
	}
};

class Test : public spk::Widget::Interface
{
private:
	Pipeline _pipeline;

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
	Test(const std::wstring &p_name) : spk::Widget::Interface(p_name),
									   _pipeline(ShaderModule("colorShader.vert"), ShaderModule("colorShader.frag"))
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