#include "graphics/graphical_device/opengl/windows/spk_opengl_windows_graphical_device.hpp"
#include "graphics/spk_window.hpp"

namespace spk::OpenGL
{
	GraphicalDevice::OpenGLStorage::OpenGLStorage(const Storage::Element& p_element) :
		AbstractGraphicalDevice::Storage(p_element)
	{
		switch (p_element.type)
		{
			case (Storage::Element::Type::Data):
			{
				_mode = GL_ARRAY_BUFFER;
				break;
			}
			case (Storage::Element::Type::Indexes):
			{
				_mode = GL_ELEMENT_ARRAY_BUFFER;
				break;
			}
			case (Storage::Element::Type::ShaderStorage):
			{
				_mode = GL_SHADER_STORAGE_BUFFER;
				break;
			}
			case (Storage::Element::Type::Texture):
			{
				_mode = GL_TEXTURE_BUFFER;
				break;
			}
		}

		glGenBuffers(1, &_vbo);
		activate();
	}

	GraphicalDevice::OpenGLStorage* GraphicalDevice::OpenGLStorage::copy() const
	{
		return (new GraphicalDevice::OpenGLStorage(_elementDescription));
	}
	
	void GraphicalDevice::OpenGLStorage::push(const spk::DataBuffer &p_bufferToPush)
	{
		_size = p_bufferToPush.size();
		glBindBuffer(_mode, _vbo);

		if (_pushedSize < p_bufferToPush.size())
		{
			glBufferData(_mode, p_bufferToPush.size(), p_bufferToPush.data(), GL_DYNAMIC_DRAW);
			_pushedSize = p_bufferToPush.size();
		}
		else
		{
			glBufferSubData(_mode, 0, p_bufferToPush.size(), p_bufferToPush.data());
		}

		glBindBuffer(_mode, 0);
	}
	
	size_t GraphicalDevice::OpenGLStorage::nbElement()
	{
		return (_size / _elementDescription.stride);
	}
	
	void GraphicalDevice::OpenGLStorage::activate()
	{
		glBindBuffer(_mode, _vbo);
		for (const auto& attribute : _elementDescription.attributes)
		{
			glEnableVertexAttribArray(attribute.location);

			glVertexAttribPointer(
				attribute.location,
				attribute.format,
				GL_FLOAT,
				GL_FALSE,
				_elementDescription.stride,
				(void*)(attribute.offset * sizeof(GL_FLOAT))
        	);
		}
	}
	
	void GraphicalDevice::OpenGLStorage::deactivate()
	{
		glBindBuffer(_mode, 0);
	}

	AbstractGraphicalDevice::Storage* GraphicalDevice::_createStorage(const Storage::Element& p_element)
	{
		return (new GraphicalDevice::OpenGLStorage(p_element));
	}
	AbstractGraphicalDevice::AbstractUniform* GraphicalDevice::_createUniform(const std::string &p_inputLine)
	{
		return (nullptr);
	}

	void compileShader(GLuint p_shaderIndex, std::string p_shaderCode)
	{
		GLint result;
		GLint len;
		const char* content = p_shaderCode.c_str();

		result = GL_FALSE;

		glShaderSource(p_shaderIndex, 1, &content, NULL);
		glCompileShader(p_shaderIndex);
		glGetShaderiv(p_shaderIndex, GL_COMPILE_STATUS, &result);

		if (result != GL_TRUE)
		{
			glGetShaderiv(p_shaderIndex, GL_INFO_LOG_LENGTH, &len);
			char* errorMsg = new char[len + 1];
			glGetShaderInfoLog(p_shaderIndex, len, NULL, errorMsg);
			spk::throwException(L"Error while compiling a shader :\n" + spk::stringToWString(errorMsg));
		}
	}

	void compileProgram(GLuint p_programID, GLuint p_vertexID, GLuint p_fragmentID)
	{
		GLint result;

		result = GL_FALSE;
		glAttachShader(p_programID, p_vertexID);
		glAttachShader(p_programID, p_fragmentID);
		glLinkProgram(p_programID);
		glGetProgramiv(p_programID, GL_LINK_STATUS, &result);
		if (result != GL_TRUE)
		{
			spk::throwException(L"Error while linking a program");
		}
		glDetachShader(p_programID, p_vertexID);
		glDetachShader(p_programID, p_fragmentID);
		glDeleteShader(p_vertexID);
		glDeleteShader(p_fragmentID);
	}

	void compile(const GLuint& p_program, const std::string & p_vertexShaderCode, const std::string & p_fragmentShaderCode)
	{
		GLuint _vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
		GLuint _fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

		compileShader(_vertex_shader_id, p_vertexShaderCode);
		compileShader(_fragment_shader_id, p_fragmentShaderCode);
		compileProgram(p_program, _vertex_shader_id, _fragment_shader_id);
	}

	void GraphicalDevice::_loadDevice(
		const std::string &p_vertexModuleName, const std::string &p_vertexModuleCode,
		const std::string &p_fragmentModuleName, const std::string &p_fragmentModuleCode)
	{
		_program = glCreateProgram();
		glGenVertexArrays(1, &_VAO);
		glBindVertexArray(_VAO);

		compile(_program, p_vertexModuleCode, p_fragmentModuleCode);
	}
	
	void GraphicalDevice::activate()
	{
		glUseProgram(_program);

		glBindVertexArray(_VAO);
	}

	void GraphicalDevice::launch(const size_t& p_nbIndexes)
	{
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_nbIndexes), GL_UNSIGNED_INT, nullptr);
	}
}