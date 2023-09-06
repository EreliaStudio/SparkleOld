#pragma once

#include "graphics/graphical_device/spk_abstract_graphical_device.hpp"

namespace spk::OpenGL
{
	class GraphicalDevice : public AbstractGraphicalDevice
	{
	public:
		class OpenGLStorage : public spk::AbstractGraphicalDevice::Storage
		{
		public:
			class OpenGLBuffer : public spk::AbstractGraphicalDevice::Storage::Buffer
			{
			private:
				spk::AbstractGraphicalDevice::Storage::Description::Buffer _description;
				GLuint _vbo;
				GLenum _mode;
				size_t _size = 0;
				size_t _pushedSize = 0;

			public:
				OpenGLBuffer(const spk::AbstractGraphicalDevice::Storage::Description::Buffer& p_bufferDescription)
				{
					_description = p_bufferDescription;
					switch (p_bufferDescription.type)
					{
						case (spk::AbstractGraphicalDevice::Storage::Description::Buffer::Type::Data):
						{
							_mode = GL_ARRAY_BUFFER;
							break;
						}
						case (spk::AbstractGraphicalDevice::Storage::Description::Buffer::Type::Indexes):
						{
							_mode = GL_ELEMENT_ARRAY_BUFFER;
							break;
						}
						case (spk::AbstractGraphicalDevice::Storage::Description::Buffer::Type::ShaderStorage):
						{
							_mode = GL_SHADER_STORAGE_BUFFER;
							break;
						}
						case (spk::AbstractGraphicalDevice::Storage::Description::Buffer::Type::Texture):
						{
							_mode = GL_TEXTURE_BUFFER;
							break;
						}
					}

					glGenBuffers(1, &_vbo);
					glBindBuffer(_mode, _vbo);
					for (const auto& attribute : _description.attributes)
					{
						glEnableVertexAttribArray(attribute.location);

						glVertexAttribPointer(
							attribute.location,
							attribute.format,
							GL_FLOAT,
							GL_FALSE,
							_description.stride,
							(void*)(attribute.offset * sizeof(GL_FLOAT))
						);
					}
				}
				OpenGLBuffer* copy() const
				{
					return (new OpenGLBuffer(_description));
				}
				void _pushDataBuffer(const spk::DataBuffer &p_bufferToPush)
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

		private:
			GLuint _VAO;

			OpenGLStorage* _duplicate()
			{
				return (new OpenGLStorage());
			}
			OpenGLBuffer* _allocateBuffer(const Description::Buffer& p_bufferDescription)
			{
				return (new OpenGLBuffer(p_bufferDescription));
			}

		public:
			OpenGLStorage()
			{
				glGenVertexArrays(1, &_VAO);
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

	private:
		GLuint _program;

		void _compileShader(GLuint p_shaderIndex, std::string p_shaderCode)
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

		void _compileProgram(GLuint p_programID, GLuint p_vertexID, GLuint p_fragmentID)
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
		}

		void _loadDevice(
			const std::string &p_vertexModuleName, const std::string &p_vertexModuleCode,
			const std::string &p_fragmentModuleName, const std::string &p_fragmentModuleCode)
		{
			_program = glCreateProgram();
			GLuint _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			GLuint _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

			_compileShader(_vertexShaderID, p_vertexModuleCode);
			_compileShader(_fragmentShaderID, p_fragmentModuleCode);
			_compileProgram(_program, _vertexShaderID, _fragmentShaderID);

			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);
		}

		OpenGLStorage *_createStorage()
		{
			return (new OpenGLStorage());
		}

	public:
		void activate()
		{
			glUseProgram(_program);
		}
		void launch(const size_t& p_nbIndexes)
		{
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_nbIndexes), GL_UNSIGNED_INT, nullptr);
		}
	};
}