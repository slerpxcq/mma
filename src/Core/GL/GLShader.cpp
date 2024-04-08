#include "mmpch.hpp"
#include "GLShader.hpp"

namespace mm
{
	GLShader::GLShader() :
		m_programId(0),
		m_shaderIds()
	{
		m_programId = glCreateProgram();
	}

	uint32_t GLShader::GetLoc(const std::string& name)
	{
		int32_t loc = -1;
		auto locIt = m_locCache.find(name);
		if (locIt != m_locCache.end())
			loc = locIt->second;
		return loc;
	}

	uint32_t GLShader::GLShaderType(uint32_t type) 
	{
		uint32_t gltype;
		switch (type) {
		case VERTEX:
			gltype = GL_VERTEX_SHADER;
			break;
		case FRAGMENT:
			gltype = GL_FRAGMENT_SHADER;
			break;
		default:
			MM_FATAL("Unknown shader type");
			MM_ASSERT(0);
		}
		return gltype;
	}

	void GLShader::DetachAllShaders() 
	{ 
		for (uint32_t& id : m_shaderIds) {
			if (id > 0) {
				glDetachShader(m_programId, id);
			}
		}
	}

	void GLShader::AttachAllShaders() 
	{ 
		for (uint32_t& id : m_shaderIds) {
			if (id > 0) {
				glAttachShader(m_programId, id);
			}
		}
	}

	void GLShader::DeleteAllShaders()
	{
		for (uint32_t& id : m_shaderIds) {
			if (id > 0) {
				glDeleteShader(id);
				id = 0;
			}
		}
	}

	void GLShader::Compile(const std::filesystem::path& path, Type type)
	{
		std::ifstream ifs(path);
		MM_ASSERT(ifs.is_open());
		std::stringstream ss;
		ss << ifs.rdbuf();
		std::string str = ss.str();

		uint32_t gltype = GLShaderType(type);
		uint32_t id = glCreateShader(gltype);
		m_shaderIds[type] = id;

		const char* cstr = str.c_str();
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
			MM_ERROR("Failed to compile shader: {0}", path.u8string().c_str());
			MM_ERROR("What: {0}", &errorLog[0]);

			DeleteAllShaders();
			return;
		}

		MM_INFO("id={0}, path={1}: shader compiled", id, path.u8string().c_str());
	} 
	void GLShader::Link()
	{
		AttachAllShaders();

		glLinkProgram(m_programId);
		GLint isLinked = 0;
		glGetProgramiv(m_programId, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_programId, maxLength, &maxLength, &infoLog[0]);
			infoLog.push_back(0);
			MM_ERROR("Failed to link program");
			MM_ERROR("What: {0}", &infoLog[0]);

			glDeleteProgram(m_programId);
			DeleteAllShaders();
		}

		DetachAllShaders();

		MM_INFO("id={0}: shader linked", m_programId);

		int32_t count;
		glGetProgramiv(m_programId, GL_ACTIVE_UNIFORMS, &count);
		for (uint32_t i = 0; i < count; ++i) {
			static constexpr uint32_t BUF_SIZE = 128;
			char buf[BUF_SIZE];
			int32_t len, size;
			uint32_t type;
			glGetActiveUniform(m_programId, i, BUF_SIZE, &len, &size, &type, buf);
			int32_t loc = glGetUniformLocation(m_programId, buf);
			if (loc >= 0) {
				m_locCache.insert({ std::string(buf), loc});
				MM_INFO("id={0}: found uniform; loc={1}, name={2}", m_programId, loc, buf);
			}
		}
	}

	void GLShader::Use() const
	{
		glUseProgram(m_programId);
	}

	GLShader::~GLShader()
	{
		DeleteAllShaders();
		glDeleteProgram(m_programId);
		MM_INFO("id={0}: shader unloaded", m_programId);
	}

	template <>
	void GLShader::UniformHelper<float>(int32_t loc, uint32_t count, const float* v)
	{
		glProgramUniform1fv(m_programId, loc, count, v);
	}

	template <>
	void GLShader::UniformHelper<glm::vec2>(int32_t loc, uint32_t count, const glm::vec2* v)
	{
		glProgramUniform2fv(m_programId, loc, count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::UniformHelper<glm::vec3>(int32_t loc, uint32_t count, const glm::vec3* v)
	{
		glProgramUniform3fv(m_programId, loc, count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::UniformHelper<glm::vec4>(int32_t loc, uint32_t count, const glm::vec4* v)
	{
		glProgramUniform4fv(m_programId, loc, count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::UniformHelper<int32_t>(int32_t loc, uint32_t count, const int32_t* v)
	{
		glProgramUniform1iv(m_programId, loc, count, v);
	}

	template <>
	void GLShader::UniformHelper<glm::ivec2>(int32_t loc, uint32_t count, const glm::ivec2 *v)
	{
		glProgramUniform2iv(m_programId, loc, count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::UniformHelper<glm::ivec3>(int32_t loc, uint32_t count, const glm::ivec3* v)
	{
		glProgramUniform3iv(m_programId, loc, count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::UniformHelper<glm::ivec4>(int32_t loc, uint32_t count, const glm::ivec4* v)
	{
		glProgramUniform4iv(m_programId, loc, count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::UniformHelper<uint32_t>(int32_t loc, uint32_t count, const uint32_t* v)
	{
		glProgramUniform1uiv(m_programId, loc, count, v);
	}

	template <>
	void GLShader::UniformHelper<glm::uvec2>(int32_t loc, uint32_t count, const glm::uvec2* v)
	{
		glProgramUniform2uiv(m_programId, loc, count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::UniformHelper<glm::uvec3>(int32_t loc, uint32_t count, const glm::uvec3* v)
	{
		glProgramUniform3uiv(m_programId, loc, count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::UniformHelper<glm::uvec4>(int32_t loc, uint32_t count, const glm::uvec4* v)
	{
		glProgramUniform4uiv(m_programId, loc, count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::UniformHelper<glm::mat4>(int32_t loc, uint32_t count, const glm::mat4* v)
	{
		glProgramUniformMatrix4fv(m_programId, loc, count, GL_FALSE, glm::value_ptr(*v));
	}
}
