#include "sparkle.hpp"

class Pipeline : public spk::GraphicalAPI::AbstractPipeline
{
public:
	class OpenGLObject : public spk::GraphicalAPI::AbstractPipeline::Object
	{
	private:
		class Aggregator
		{
		private:
			spk::GraphicalAPI::AbstractPipeline* _owner;
			GLuint _VAO;

		public:
			Aggregator(spk::GraphicalAPI::AbstractPipeline* p_owner) :
				_owner(p_owner)
			{
				_owner->activate();
				glGenVertexArrays(1, &_VAO);
				activate();
			}

			void activate()
			{
				glBindVertexArray(_VAO);
			}

			void deactivate()
			{
				glBindVertexArray(0);
			}
		};

		class Buffer
		{
		private:
			GLuint _vbo;
			GLenum _mode;
			size_t _size = 0;
			size_t _pushedSize = 0;

		public:
			Buffer(const spk::GraphicalAPI::AbstractPipeline::Object::Storage::Configuration& p_configuration)
			{
				_mode = spk::GraphicalAPI::convertModeToGLenum(p_configuration.mode);

				glGenBuffers(1, &_vbo);
				glBindBuffer(_mode, _vbo);
				for (const auto& attribute : p_configuration.attributes)
				{
					glEnableVertexAttribArray(attribute.second.location);

					glVertexAttribPointer(
						attribute.second.location,
						attribute.second.size,
						spk::GraphicalAPI::convertTypeToGLenum(attribute.second.type),
						GL_FALSE,
						p_configuration.stride,
						(void *)(attribute.second.offset));
				}
			}

			void push(const void *data, const size_t dataSize)
			{
				activate();
				_size = dataSize;
				glBindBuffer(_mode, _vbo);

				if (_pushedSize < dataSize)
				{
					glBufferData(_mode, dataSize, data, GL_DYNAMIC_DRAW);
					_pushedSize = dataSize;
				}
				else
				{
					glBufferSubData(_mode, 0, dataSize, data);
				}
			}

			void activate()
			{
				glBindBuffer(_mode, _vbo);
			}

			void deactivate()
			{
				glBindBuffer(_mode, 0);
			}
		};

		Aggregator _aggregator;
		Buffer _modelBuffer;
		Buffer _indexesBuffer;

	public:
		OpenGLObject(spk::GraphicalAPI::AbstractPipeline* p_owner, const spk::GraphicalAPI::AbstractPipeline::Object::Storage::Configuration& p_storageConfiguration) :
			spk::GraphicalAPI::AbstractPipeline::Object(p_owner, p_storageConfiguration),
			_aggregator(p_owner),
			_modelBuffer(p_storageConfiguration),
			_indexesBuffer(spk::GraphicalAPI::AbstractPipeline::Object::Storage::Configuration::Mode::Indexes)
		{
		}

		void push()
		{
			_aggregator.activate();
			_modelBuffer.push(storage().data(), storage().size());
			_indexesBuffer.push(indexes().data(), indexes().size() * sizeof(unsigned int));
			_aggregator.deactivate();
		}

		void activate()
		{
			_aggregator.activate();
			_modelBuffer.activate();
			_indexesBuffer.activate();
		}
		
		void deactivate()
		{
			_aggregator.deactivate();
			_modelBuffer.deactivate();
			_indexesBuffer.deactivate();
		}
	};

private:
	GLuint _program;

	void _loadProgram(
		const std::string& p_vertexName, const std::string& p_vertexCode,
		const std::string& p_fragmentName, const std::string& p_fragmentCode)
	{
		_program = glCreateProgram();
		GLuint _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		spk::GraphicalAPI::compileShaderModule(_vertexShaderID, p_vertexCode);
		spk::GraphicalAPI::compileShaderModule(_fragmentShaderID, p_fragmentCode);
		spk::GraphicalAPI::compileProgram(_program, _vertexShaderID, _fragmentShaderID);

		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
		activate();
	}

public: 
	Pipeline(const std::filesystem::path& p_vertexShaderPath, const std::filesystem::path& p_fragmentShaderPath) :
		spk::GraphicalAPI::AbstractPipeline()
	{
		loadFromFile(p_vertexShaderPath, p_fragmentShaderPath);
	}

	void activate()
	{
		glUseProgram(_program);
	}

	void deactivate() 
	{
		glUseProgram(0);
	}

	void launch(const size_t &p_nbIndexes)
	{
		spk::GraphicalAPI::checkOpengl(__METHOD__ + std::to_wstring(__LINE__));
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_nbIndexes), GL_UNSIGNED_INT, nullptr);
		spk::GraphicalAPI::checkOpengl(__METHOD__ + std::to_wstring(__LINE__));
	}
	
	std::shared_ptr<Object> createObject()
	{
		return (std::make_shared<OpenGLObject>(this, _storageConfiguration));
	}
};

class Test : public spk::Widget::Interface
{
private:
	Pipeline _pipeline;
	std::shared_ptr<Pipeline::Object> _object;

	void _onGeometryChange()
	{		
		/*
			Fill the object
		*/
		std::vector<Pipeline::Object::Storage::Unit<spk::Vector2>> datas = {
			{spk::Vector2( 0.0f,  1.0f)},
			{spk::Vector2(-1.0f, -1.0f)},
			{spk::Vector2( 1.0f, -1.0f)}
		};
		// std::vector<Pipeline::Object::Storage::Unit<spk::Vector2, spk::Color>> datas = {
		// 	{spk::Vector2( 0.0f,  1.0f), spk::Color(255, 0, 0)},
		// 	{spk::Vector2(-1.0f, -1.0f), spk::Color(0, 255, 0)},
		// 	{spk::Vector2( 1.0f, -1.0f), spk::Color(0, 0, 255)}
		// };
		std::vector<unsigned int> indexes = {
			0, 1, 2
		};
		
		_object->storage() << datas;
		_object->indexes() << indexes;

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