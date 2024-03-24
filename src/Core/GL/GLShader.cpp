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
		uint32_t loc;
		try {
			loc = m_locCache.at(name);
		}
		catch (const std::out_of_range& e) {
			loc = glGetUniformLocation(m_programId, name.c_str());
			MM_ASSERT(loc > 0);
			m_locCache.insert({ name, loc });
		}
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
			MM_ERROR("Failed to compile shader: \n  Type: {0}", type);
			MM_ERROR("What: {0}", &errorLog[0]);

			DeleteAllShaders();
		}
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
	}

	void GLShader::Use()
	{
		glUseProgram(m_programId);
	}

	template <>
	void GLShader::Uniform<float>(const std::string& name, uint32_t count, const float* v)
	{
		glUniform1fv(GetLoc(name), count, v);
	}

	template <>
	void GLShader::Uniform<glm::vec2>(const std::string& name, uint32_t count, const glm::vec2* v)
	{
		glUniform2fv(GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<glm::vec3>(const std::string& name, uint32_t count, const glm::vec3* v)
	{
		glUniform3fv(GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<glm::vec4>(const std::string& name, uint32_t count, const glm::vec4* v)
	{
		glUniform4fv(GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<int32_t>(const std::string& name, uint32_t count, const int32_t* v)
	{
		glUniform1iv(GetLoc(name), count, v);
	}

	template <>
	void GLShader::Uniform<glm::ivec2>(const std::string& name, uint32_t count, const glm::ivec2 *v)
	{
		glUniform2iv(GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<glm::ivec3>(const std::string& name, uint32_t count, const glm::ivec3* v)
	{
		glUniform3iv(GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<glm::ivec4>(const std::string& name, uint32_t count, const glm::ivec4* v)
	{
		glUniform4iv(GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<uint32_t>(const std::string& name, uint32_t count, const uint32_t* v)
	{
		glUniform1uiv(GetLoc(name), count, v);
	}

	template <>
	void GLShader::Uniform<glm::uvec2>(const std::string& name, uint32_t count, const glm::uvec2* v)
	{
		glUniform2uiv(GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<glm::uvec3>(const std::string& name, uint32_t count, const glm::uvec3* v)
	{
		glUniform3uiv(GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<glm::uvec4>(const std::string& name, uint32_t count, const glm::uvec4* v)
	{
		glUniform4uiv(GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<glm::mat4>(const std::string& name, uint32_t count, const glm::mat4* v)
	{
		glUniformMatrix4fv(GetLoc(name), count, GL_FALSE, glm::value_ptr(*v));
	}

	GLShader::~GLShader()
	{
		DeleteAllShaders();
		glDeleteProgram(m_programId);
	}
}
