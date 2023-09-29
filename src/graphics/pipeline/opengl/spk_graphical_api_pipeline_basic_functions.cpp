#include "graphics/pipeline/opengl/spk_graphical_api_pipeline_basic_functions.hpp"

namespace spk::GraphicalAPI
{
	void compileShaderModule(GLuint p_shaderIndex, std::string p_shaderCode)
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
	}

	std::wstring convertGLEnumToWString(const GLenum& p_type)
	{
		switch (p_type)
		{
		case GL_NO_ERROR: return (L"No error");
		case GL_INVALID_ENUM: return (L"Invalid enum");
		case GL_INVALID_VALUE: return (L"Invalid value");
		case GL_INVALID_OPERATION: return (L"Invalid operation");
		case GL_INVALID_FRAMEBUFFER_OPERATION: return (L"Invalid framebuffer operation");
		case GL_OUT_OF_MEMORY: return (L"Out of memory");
		case GL_STACK_UNDERFLOW: return (L"Stack underflow");
		case GL_STACK_OVERFLOW: return (L"Stack overflow");

		case GL_BYTE: return (L"GLbyte");
		case GL_UNSIGNED_BYTE: return (L"GLubyte");
		case GL_UNSIGNED_SHORT: return (L"GLushort");
		case GL_2_BYTES: return (L"GL_2_BYTES");
		case GL_3_BYTES: return (L"GL_3_BYTES");
		case GL_4_BYTES: return (L"GL_4_BYTES");

		case 	GL_FLOAT: return (L"float");
		case 	GL_FLOAT_VEC2: return (L"vec2");
		case 	GL_FLOAT_VEC3: return (L"vec3");
		case 	GL_FLOAT_VEC4: return (L"vec4");
		case 	GL_DOUBLE: return (L"double");
		case 	GL_DOUBLE_VEC2: return (L"dvec2");
		case 	GL_DOUBLE_VEC3: return (L"dvec3");
		case 	GL_DOUBLE_VEC4: return (L"dvec4");
		case 	GL_INT: return (L"int");
		case 	GL_INT_VEC2: return (L"ivec2");
		case 	GL_INT_VEC3: return (L"ivec3");
		case 	GL_INT_VEC4: return (L"ivec4");
		case 	GL_UNSIGNED_INT: return (L"unsigned int");
		case 	GL_UNSIGNED_INT_VEC2: return (L"uvec2");
		case 	GL_UNSIGNED_INT_VEC3: return (L"uvec3");
		case 	GL_UNSIGNED_INT_VEC4: return (L"uvec4");
		case 	GL_BOOL: return (L"bool");
		case 	GL_BOOL_VEC2: return (L"bvec2");
		case 	GL_BOOL_VEC3: return (L"bvec3");
		case 	GL_BOOL_VEC4: return (L"bvec4");
		case 	GL_FLOAT_MAT2: return (L"mat2");
		case 	GL_FLOAT_MAT3: return (L"mat3");
		case 	GL_FLOAT_MAT4: return (L"mat4");
		case 	GL_FLOAT_MAT2x3: return (L"mat2x3");
		case 	GL_FLOAT_MAT2x4: return (L"mat2x4");
		case 	GL_FLOAT_MAT3x2: return (L"mat3x2");
		case 	GL_FLOAT_MAT3x4: return (L"mat3x4");
		case 	GL_FLOAT_MAT4x2: return (L"mat4x2");
		case 	GL_FLOAT_MAT4x3: return (L"mat4x3");
		case 	GL_DOUBLE_MAT2: return (L"dmat2");
		case 	GL_DOUBLE_MAT3: return (L"dmat3");
		case 	GL_DOUBLE_MAT4: return (L"dmat4");
		case 	GL_DOUBLE_MAT2x3: return (L"dmat2x3");
		case 	GL_DOUBLE_MAT2x4: return (L"dmat2x4");
		case 	GL_DOUBLE_MAT3x2: return (L"dmat3x2");
		case 	GL_DOUBLE_MAT3x4: return (L"dmat3x4");
		case 	GL_DOUBLE_MAT4x2: return (L"dmat4x2");
		case 	GL_DOUBLE_MAT4x3: return (L"dmat4x3");
		case 	GL_SAMPLER_1D: return (L"sampler1D");
		case 	GL_SAMPLER_2D: return (L"sampler2D");
		case 	GL_SAMPLER_3D: return (L"sampler3D");
		case 	GL_SAMPLER_CUBE: return (L"samplerCube");
		case 	GL_SAMPLER_1D_SHADOW: return (L"sampler1DShadow");
		case 	GL_SAMPLER_2D_SHADOW: return (L"sampler2DShadow");
		case 	GL_SAMPLER_1D_ARRAY: return (L"sampler1DArray");
		case 	GL_SAMPLER_2D_ARRAY: return (L"sampler2DArray");
		case 	GL_SAMPLER_1D_ARRAY_SHADOW: return (L"sampler1DArrayShadow");
		case 	GL_SAMPLER_2D_ARRAY_SHADOW: return (L"sampler2DArrayShadow");
		case 	GL_SAMPLER_2D_MULTISAMPLE: return (L"sampler2DMS");
		case 	GL_SAMPLER_2D_MULTISAMPLE_ARRAY: return (L"sampler2DMSArray");
		case 	GL_SAMPLER_CUBE_SHADOW: return (L"samplerCubeShadow");
		case 	GL_SAMPLER_BUFFER: return (L"samplerBuffer");
		case 	GL_SAMPLER_2D_RECT: return (L"sampler2DRect");
		case 	GL_SAMPLER_2D_RECT_SHADOW: return (L"sampler2DRectShadow");
		case 	GL_INT_SAMPLER_1D: return (L"isampler1D");
		case 	GL_INT_SAMPLER_2D: return (L"isampler2D");
		case 	GL_INT_SAMPLER_3D: return (L"isampler3D");
		case 	GL_INT_SAMPLER_CUBE: return (L"isamplerCube");
		case 	GL_INT_SAMPLER_1D_ARRAY: return (L"isampler1DArray");
		case 	GL_INT_SAMPLER_2D_ARRAY: return (L"isampler2DArray");
		case 	GL_INT_SAMPLER_2D_MULTISAMPLE: return (L"isampler2DMS");
		case 	GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return (L"isampler2DMSArray");
		case 	GL_INT_SAMPLER_BUFFER: return (L"isamplerBuffer");
		case 	GL_INT_SAMPLER_2D_RECT: return (L"isampler2DRect");
		case 	GL_UNSIGNED_INT_SAMPLER_1D: return (L"usampler1D");
		case 	GL_UNSIGNED_INT_SAMPLER_2D: return (L"usampler2D");
		case 	GL_UNSIGNED_INT_SAMPLER_3D: return (L"usampler3D");
		case 	GL_UNSIGNED_INT_SAMPLER_CUBE: return (L"usamplerCube");
		case 	GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: return (L"usampler2DArray");
		case 	GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: return (L"usampler2DArray");
		case 	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: return (L"usampler2DMS");
		case 	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return (L"usampler2DMSArray");
		case 	GL_UNSIGNED_INT_SAMPLER_BUFFER: return (L"usamplerBuffer");
		case 	GL_UNSIGNED_INT_SAMPLER_2D_RECT: return (L"usampler2DRect");
		case 	GL_IMAGE_1D: return (L"image1D");
		case 	GL_IMAGE_2D: return (L"image2D");
		case 	GL_IMAGE_3D: return (L"image3D");
		case 	GL_IMAGE_2D_RECT: return (L"image2DRect");
		case 	GL_IMAGE_CUBE: return (L"imageCube");
		case 	GL_IMAGE_BUFFER: return (L"imageBuffer");
		case 	GL_IMAGE_1D_ARRAY: return (L"image1DArray");
		case 	GL_IMAGE_2D_ARRAY: return (L"image2DArray");
		case 	GL_IMAGE_2D_MULTISAMPLE: return (L"image2DMS");
		case 	GL_IMAGE_2D_MULTISAMPLE_ARRAY: return (L"image2DMSArray");
		case 	GL_INT_IMAGE_1D: return (L"iimage1D");
		case 	GL_INT_IMAGE_2D: return (L"iimage2D");
		case 	GL_INT_IMAGE_3D: return (L"iimage3D");
		case 	GL_INT_IMAGE_2D_RECT: return (L"iimage2DRect");
		case 	GL_INT_IMAGE_CUBE: return (L"iimageCube");
		case 	GL_INT_IMAGE_BUFFER: return (L"iimageBuffer");
		case 	GL_INT_IMAGE_1D_ARRAY: return (L"iimage1DArray");
		case 	GL_INT_IMAGE_2D_ARRAY: return (L"iimage2DArray");
		case 	GL_INT_IMAGE_2D_MULTISAMPLE: return (L"iimage2DMS");
		case 	GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return (L"iimage2DMSArray");
		case 	GL_UNSIGNED_INT_IMAGE_1D: return (L"uimage1D");
		case 	GL_UNSIGNED_INT_IMAGE_2D: return (L"uimage2D");
		case 	GL_UNSIGNED_INT_IMAGE_3D: return (L"uimage3D");
		case 	GL_UNSIGNED_INT_IMAGE_2D_RECT: return (L"uimage2DRect");
		case 	GL_UNSIGNED_INT_IMAGE_CUBE: return (L"uimageCube");
		case 	GL_UNSIGNED_INT_IMAGE_BUFFER: return (L"uimageBuffer");
		case 	GL_UNSIGNED_INT_IMAGE_1D_ARRAY: return (L"uimage1DArray");
		case 	GL_UNSIGNED_INT_IMAGE_2D_ARRAY: return (L"uimage2DArray");
		case 	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: return (L"uimage2DMS");
		case 	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return (L"uimage2DMSArray");
		case 	GL_UNSIGNED_INT_ATOMIC_COUNTER: return (L"atomic_uint");

 		case GL_ARRAY_BUFFER: return L"Vertex attributes";
        case GL_ATOMIC_COUNTER_BUFFER: return L"Atomic counter storage";
        case GL_COPY_READ_BUFFER: return L"Buffer copy source";
        case GL_COPY_WRITE_BUFFER: return L"Buffer copy destination";
        case GL_DISPATCH_INDIRECT_BUFFER: return L"Indirect compute dispatch commands";
        case GL_DRAW_INDIRECT_BUFFER: return L"Indirect command arguments";
        case GL_ELEMENT_ARRAY_BUFFER: return L"Vertex array indices";
        case GL_PIXEL_PACK_BUFFER: return L"Pixel read target";
        case GL_PIXEL_UNPACK_BUFFER: return L"Texture data source";
        case GL_QUERY_BUFFER: return L"Query result buffer";
        case GL_SHADER_STORAGE_BUFFER: return L"Read-write storage for shaders";
        case GL_TEXTURE_BUFFER: return L"Texture data buffer";
        case GL_TRANSFORM_FEEDBACK_BUFFER: return L"Transform feedback buffer";
        case GL_UNIFORM_BUFFER: return L"Uniform block storage";

		default: return (L"not a GLenum type");
		}
	}

	void checkOpengl(const std::wstring& msg)
	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			spk::throwException(msg + L" : type [" + convertGLEnumToWString(err) + L"]");
		}
		else
		{
			spk::cout << msg << std::endl;
		}
	}
}