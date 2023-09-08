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
				GLuint _vbo;
				GLenum _mode;
				GLenum _type;
				size_t _size = 0;
				size_t _pushedSize = 0;

				GLenum _convertTypeToGLenum(const spk::AbstractGraphicalDevice::Storage::Description::Buffer::Type &p_input)
				{
					switch (p_input)
					{
					case (spk::AbstractGraphicalDevice::Storage::Description::Buffer::Type::Float):
						return (GL_FLOAT);
					case (spk::AbstractGraphicalDevice::Storage::Description::Buffer::Type::UInt):
						return (GL_UNSIGNED_INT);
					case (spk::AbstractGraphicalDevice::Storage::Description::Buffer::Type::Int):
						return (GL_INT);
					}
					spk::throwException(L"Unexpected buffer type");
					return (GL_NONE);
				}

			public:
				OpenGLBuffer(const spk::AbstractGraphicalDevice::Storage::Description::Buffer &p_bufferDescription) : spk::AbstractGraphicalDevice::Storage::Buffer(p_bufferDescription)
				{
					switch (description().mode)
					{
					case (spk::AbstractGraphicalDevice::Storage::Description::Buffer::Mode::Data):
					{
						_mode = GL_ARRAY_BUFFER;
						break;
					}
					case (spk::AbstractGraphicalDevice::Storage::Description::Buffer::Mode::Indexes):
					{
						_mode = GL_ELEMENT_ARRAY_BUFFER;
						break;
					}
					case (spk::AbstractGraphicalDevice::Storage::Description::Buffer::Mode::ShaderStorage):
					{
						_mode = GL_SHADER_STORAGE_BUFFER;
						break;
					}
					case (spk::AbstractGraphicalDevice::Storage::Description::Buffer::Mode::Texture):
					{
						_mode = GL_TEXTURE_BUFFER;
						break;
					}
					}

					glGenBuffers(1, &_vbo);
					glBindBuffer(_mode, _vbo);
					for (size_t i = 0; i < description().attributes.size(); i++)
					{
						const auto &attribute = description().attributes[i];
						glEnableVertexAttribArray(attribute.location);

						glVertexAttribPointer(
							attribute.location,
							attribute.format,
							_convertTypeToGLenum(attribute.type),
							GL_FALSE,
							description().stride,
							(void *)(attribute.offset));
					}
				}
				OpenGLBuffer *copy() const
				{
					return (new OpenGLBuffer(description()));
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

			class OpenGLDescriptor : public spk::AbstractGraphicalDevice::Storage::Descriptor
			{
			public:
				class OpenGLUniform : public spk::AbstractGraphicalDevice::Storage::Descriptor::Uniform
				{
				private:
					uint32_t _location = 0;

					void _push(float p_value)
					{
						glUniform1f(_location, p_value);
					}

					void _push(int p_value)
					{
						glUniform1i(_location, p_value);
					}

					void _push(unsigned int p_value)
					{
						glUniform1ui(_location, p_value);
					}

					void _push(const spk::Vector2 &p_value)
					{
						glUniform2f(_location, p_value.x, p_value.y);
					}

					void _push(const spk::Vector2Int &p_value)
					{
						glUniform2i(_location, p_value.x, p_value.y);
					}

					void _push(const spk::Vector2UInt &p_value)
					{
						glUniform2ui(_location, p_value.x, p_value.y);
					}

					void _push(const spk::Vector3 &p_value)
					{
						glUniform3f(_location, p_value.x, p_value.y, p_value.z);
					}

					void _push(const spk::Vector3Int &p_value)
					{
						glUniform3i(_location, p_value.x, p_value.y, p_value.z);
					}

					void _push(const spk::Vector3UInt &p_value)
					{
						glUniform3ui(_location, p_value.x, p_value.y, p_value.z);
					}

					void _push(const spk::Color &p_value)
					{
						glUniform4f(_location, p_value.r, p_value.g, p_value.b, p_value.a);
					}

					void _push(const spk::Matrix4x4 &p_value)
					{
						glUniformMatrix4fv(_location, 1, GL_FALSE, p_value.c_ptr());
					}

				public:
					OpenGLUniform(const GLuint &p_program, const Description::Descriptor::Uniform &p_uniformDescription) : spk::AbstractGraphicalDevice::Storage::Descriptor::Uniform(p_uniformDescription),
																														   _location(glGetUniformLocation(p_program, spk::wstringToString(p_uniformDescription.name).c_str()))
					{
					}
				};

			private:
				GLuint _program;

				Descriptor *_copy() const
				{
					return (new OpenGLDescriptor(_program, description()));
				}

				Uniform *_allocateUniform(const Description::Descriptor::Uniform &p_description)
				{
					return (new OpenGLUniform(_program, p_description));
				}

			public:
				OpenGLDescriptor(const GLuint &p_program, const Description::Descriptor &p_descriptorDescription) : spk::AbstractGraphicalDevice::Storage::Descriptor(p_descriptorDescription),
																													_program(p_program)
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
			GLuint _VAO;
			GLuint _program;

			OpenGLStorage *_duplicate()
			{
				return (new OpenGLStorage(_program));
			}
			OpenGLBuffer *_allocateBuffer(const Description::Buffer &p_bufferDescription)
			{
				return (new OpenGLBuffer(p_bufferDescription));
			}
			OpenGLDescriptor *_allocateDescriptor(const Description::Descriptor &p_bufferDescription)
			{
				glUseProgram(_program);
				return (new OpenGLDescriptor(_program, p_bufferDescription));
			}

		public:
			OpenGLStorage(const GLuint &p_program) : _program(p_program)
			{
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

	private:
		GLuint _program;

		void _compileShader(GLuint p_shaderIndex, std::string p_shaderCode)
		{
			GLint result;
			GLint len;
			const char *content = p_shaderCode.c_str();

			result = GL_FALSE;

			glShaderSource(p_shaderIndex, 1, &content, NULL);
			glCompileShader(p_shaderIndex);
			glGetShaderiv(p_shaderIndex, GL_COMPILE_STATUS, &result);

			if (result != GL_TRUE)
			{
				glGetShaderiv(p_shaderIndex, GL_INFO_LOG_LENGTH, &len);
				char *errorMsg = new char[len + 1];
				glGetShaderInfoLog(p_shaderIndex, len, NULL, errorMsg);
				spk::throwException(L"Error while compiling a shader :\n" + spk::to_wstring(errorMsg));
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
			return (new OpenGLStorage(_program));
		}

	public:
		void activate()
		{
			glUseProgram(_program);
		}
		void launch(const size_t &p_nbIndexes)
		{
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_nbIndexes), GL_UNSIGNED_INT, nullptr);
		}
	};
}