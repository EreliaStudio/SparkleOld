#include "playground.hpp"

class Pipeline : public spk::AbstractPipeline
{
public:
	class OpenGLObject : public spk::AbstractPipeline::Object
	{
	private:
		void _pushStorageDatas(const void *p_data, size_t p_size)
		{

		}

		void _pushConstantsDatas(const void *p_data, size_t p_size)
		{

		}
		
	public:
		OpenGLObject(Pipeline* p_owner, const spk::ShaderLayout::StorageBufferLayout& p_storageLayout, const spk::ShaderLayout::PushConstantLayout& p_pushConstantsLayout) : 
			spk::AbstractPipeline::Object(p_owner, p_storageLayout, p_pushConstantsLayout)
		{

		}

		void render()
		{

		}
	};

	class OpenGLUniformBlock : public spk::AbstractPipeline::UniformBlock
	{
	private:
		void _updateData(const void* p_data, size_t p_dataSize)
		{

		}

	public:
		OpenGLUniformBlock(const spk::ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) :
			spk::AbstractPipeline::UniformBlock(p_uniformBlockLayout)
		{

		}

		void activate()
		{

		}

		void deactivate()
		{

		}
	};

private:
	
	std::shared_ptr<UniformBlock> _loadUniformBlock(const spk::ShaderLayout::UniformBlockLayout& p_uniformBlockLayout)
	{
		return (std::make_shared<OpenGLUniformBlock>(p_uniformBlockLayout));
	}

	std::shared_ptr<Object> _loadObject(const spk::ShaderLayout::StorageBufferLayout& p_storageLayout, const spk::ShaderLayout::PushConstantLayout& p_pushConstantsLayout)
	{
		return (std::make_shared<OpenGLObject>(this, p_storageLayout, p_pushConstantsLayout));
	}

	void _loadProgram(const spk::ShaderLayout& p_shaderLayout)
	{

	}

public:
	Pipeline(const spk::ShaderModule &p_vertexInput, const spk::ShaderModule &p_fragmentInput) :
		AbstractPipeline(p_vertexInput, p_fragmentInput)
	{
		_loadPipeline();
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
									   _pipeline(spk::ShaderModule("colorShader.vert"), spk::ShaderModule("colorShader.frag"))
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