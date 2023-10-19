#pragma once

#include "graphics/pipeline/spk_shader_layout.hpp"
#include "spk_system_define.hpp"

namespace spk::GPU
{
	class Aggregator
	{
	private:
		GLuint _VAO;

	public:
		Aggregator();

		void activate();
		void deactivate();
	};

	class Buffer
	{
	public:
		enum class Mode
		{
			Vertices,
			Indexes,
			PushConstant
		};

	protected:
		GLuint _vbo;
		GLenum _mode;
		size_t _size = 0;
		size_t _pushedSize = 0;

		GLenum _convertModeToGLenum(const Mode &p_input);

	public:
		Buffer(const Mode &p_mode);

		virtual void push(const void *data, const size_t dataSize);

		virtual void activate();
		virtual void deactivate();
	};
}