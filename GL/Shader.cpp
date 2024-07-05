#include "RendererPch.hpp"
#include "Shader.hpp"

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mm
{

Shader::Shader()
{
	m_rendererID = glCreateProgram();
	MM_INFO("Shader program loaded successfully; id=%u", m_rendererID);
}

Shader::~Shader()
{
	glDeleteProgram(m_rendererID);
	MM_INFO("Shader program unloaded successfully; id=%u", m_rendererID);
}

bool Shader::Compile(const std::string& source, uint32_t type)
{
	uint32_t id = glCreateShader(type);
	m_shaders.push_back(id);

	const char* cstr = source.c_str();
	glShaderSource(id, 1, &cstr, nullptr);
	glCompileShader(id);

	GLint isCompiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
		errorLog.push_back(0);
		MM_ERROR("Failed to compile %s shader", ((type == GL_VERTEX_SHADER) ? "vertex" :
		                                           (type == GL_FRAGMENT_SHADER) ? "fragment" : ""));
		MM_ERROR("What: %s", &errorLog[0]);

		DeleteAllShaders();
		return false;
	}

	MM_INFO("Shader compiled successfully; id=%u", id);
	return true;
}

bool Shader::Link() 
{
	AttachAllShaders();

	glLinkProgram(m_rendererID);
	GLint isLinked = 0;
	glGetProgramiv(m_rendererID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_rendererID, maxLength, &maxLength, &infoLog[0]);
		infoLog.push_back(0);
		MM_ERROR("Failed to link program; id=%u", m_rendererID);
		MM_ERROR("What: %s", &infoLog[0]);

		glDeleteProgram(m_rendererID);
		DeleteAllShaders();
		return false;
	}

	DeleteAllShaders();

	MM_INFO("Shader program linked successfully", m_rendererID);

	LoadLocationCache();
	return true;
}

void Shader::Use() const
{
	glUseProgram(m_rendererID);
}

void Shader::DeleteAllShaders()
{
	for (uint32_t shader : m_shaders) {
		glDeleteShader(shader);
	}
	m_shaders.clear();
}

void Shader::AttachAllShaders()
{
	for (uint32_t shader : m_shaders) {
		glAttachShader(m_rendererID, shader);
	}
}

void Shader::LoadLocationCache()
{
	int32_t count;
	glGetProgramiv(m_rendererID, GL_ACTIVE_UNIFORMS, &count);
	for (uint32_t i = 0; i < count; ++i) {
		static constexpr uint32_t BUF_SIZE = 128;
		char buf[BUF_SIZE];
		int32_t len, size;
		uint32_t type;
		glGetActiveUniform(m_rendererID, i, BUF_SIZE, &len, &size, &type, buf);
		int32_t loc = glGetUniformLocation(m_rendererID, buf);
		if (loc >= 0) {
			m_locationCache.insert({ std::string(buf), loc});
			MM_INFO("Shader program found uniform; id=%u, location=%i, name=%s", m_rendererID, loc, buf);
		}
	}
}

template <>
void Shader::UniformHelper<float>(int32_t loc, uint32_t count, const float* v)
{
	glProgramUniform1fv(m_rendererID, loc, count, v);
}

template <>
void Shader::UniformHelper<glm::vec2>(int32_t loc, uint32_t count, const glm::vec2* v)
{
	glProgramUniform2fv(m_rendererID, loc, count, glm::value_ptr(*v));
}

template <>
void Shader::UniformHelper<glm::vec3>(int32_t loc, uint32_t count, const glm::vec3* v)
{
	glProgramUniform3fv(m_rendererID, loc, count, glm::value_ptr(*v));
}

template <>
void Shader::UniformHelper<glm::vec4>(int32_t loc, uint32_t count, const glm::vec4* v)
{
	glProgramUniform4fv(m_rendererID, loc, count, glm::value_ptr(*v));
}

template <>
void Shader::UniformHelper<int32_t>(int32_t loc, uint32_t count, const int32_t* v)
{
	glProgramUniform1iv(m_rendererID, loc, count, v);
}

template <>
void Shader::UniformHelper<glm::ivec2>(int32_t loc, uint32_t count, const glm::ivec2 *v)
{
	glProgramUniform2iv(m_rendererID, loc, count, glm::value_ptr(*v));
}

template <>
void Shader::UniformHelper<glm::ivec3>(int32_t loc, uint32_t count, const glm::ivec3* v)
{
	glProgramUniform3iv(m_rendererID, loc, count, glm::value_ptr(*v));
}

template <>
void Shader::UniformHelper<glm::ivec4>(int32_t loc, uint32_t count, const glm::ivec4* v)
{
	glProgramUniform4iv(m_rendererID, loc, count, glm::value_ptr(*v));
}

template <>
void Shader::UniformHelper<uint32_t>(int32_t loc, uint32_t count, const uint32_t* v)
{
	glProgramUniform1uiv(m_rendererID, loc, count, v);
}

template <>
void Shader::UniformHelper<glm::uvec2>(int32_t loc, uint32_t count, const glm::uvec2* v)
{
	glProgramUniform2uiv(m_rendererID, loc, count, glm::value_ptr(*v));
}

template <>
void Shader::UniformHelper<glm::uvec3>(int32_t loc, uint32_t count, const glm::uvec3* v)
{
	glProgramUniform3uiv(m_rendererID, loc, count, glm::value_ptr(*v));
}

template <>
void Shader::UniformHelper<glm::uvec4>(int32_t loc, uint32_t count, const glm::uvec4* v)
{
	glProgramUniform4uiv(m_rendererID, loc, count, glm::value_ptr(*v));
}

template <>
void Shader::UniformHelper<glm::mat4>(int32_t loc, uint32_t count, const glm::mat4* v)
{
	glProgramUniformMatrix4fv(m_rendererID, loc, count, GL_FALSE, glm::value_ptr(*v));
}

}
