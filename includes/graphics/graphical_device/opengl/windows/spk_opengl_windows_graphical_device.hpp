#pragma once

#include "graphics/graphical_device/spk_abstract_graphical_device.hpp"

namespace spk::OpenGL
{
	class GraphicalDevice : public AbstractGraphicalDevice
	{
	public:
		class OpenGLStorage : public spk::AbstractGraphicalDevice::Storage
		{
		private:
			GLuint _vbo;
			GLenum _mode;
			size_t _size = 0;
			size_t _pushedSize = 0;

		public:
			OpenGLStorage(const Storage::Element& p_element);
			OpenGLStorage* copy() const;
			void push(const spk::DataBuffer &p_bufferToPush);
			void activate();
			void deactivate();
			size_t nbElement();
		};

		class Uniform : public spk::AbstractGraphicalDevice::AbstractUniform
		{
		private:

		public:

		};

	private:
		GLuint _program;
		GLuint _VAO;

		Storage* _createStorage(const Storage::Element& p_element);
		AbstractUniform* _createUniform(const std::string &p_inputLine);

		void _loadDevice(
			const std::string &p_vertexModuleName, const std::string &p_vertexModuleCode,
			const std::string &p_fragmentModuleName, const std::string &p_fragmentModuleCode);

	public:
		void activate();
		void launch(const size_t& p_nbIndexes);
	};
}