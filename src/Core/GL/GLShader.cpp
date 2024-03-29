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
		int32_t loc = 0;
		try {
			loc = m_locCache.at(name);
		}
		catch (const std::out_of_range& e) {
			loc = glGetUniformLocation(m_programId, name.c_str());
	//		MM_ASSERT(loc > 0 && "Uniform does not existing in the shader");
			if (loc >= 0)
				m_locCache.insert({ name, loc });
			else
				MM_WARN("Uniform \"{0}\" does not exist in shader {1}", name, m_programId);
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
			MM_ERROR("Failed to compile shader: {0}", path.u8string().c_str());
			MM_ERROR("What: {0}", &errorLog[0]);

			DeleteAllShaders();
			return;
		}

		MM_INFO("id={0}, path={1}: Shader compiled", id, path.u8string().c_str());
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

		MM_INFO("id={0}: Shader linked", m_programId);
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
	void GLShader::Uniform<float>(const std::string& name, uint32_t count, const float* v)
	{
		glProgramUniform1fv(m_programId, GetLoc(name), count, v);
	}

	template <>
	void GLShader::Uniform<glm::vec2>(const std::string& name, uint32_t count, const glm::vec2* v)
	{
		glProgramUniform2fv(m_programId, GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<glm::vec3>(const std::string& name, uint32_t count, const glm::vec3* v)
	{
		glProgramUniform3fv(m_programId, GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<glm::vec4>(const std::string& name, uint32_t count, const glm::vec4* v)
	{
		glProgramUniform4fv(m_programId, GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<int32_t>(const std::string& name, uint32_t count, const int32_t* v)
	{
		glProgramUniform1iv(m_programId, GetLoc(name), count, v);
	}

	template <>
	void GLShader::Uniform<glm::ivec2>(const std::string& name, uint32_t count, const glm::ivec2 *v)
	{
		glProgramUniform2iv(m_programId, GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<glm::ivec3>(const std::string& name, uint32_t count, const glm::ivec3* v)
	{
		glProgramUniform3iv(m_programId, GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<glm::ivec4>(const std::string& name, uint32_t count, const glm::ivec4* v)
	{
		glProgramUniform4iv(m_programId, GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<uint32_t>(const std::string& name, uint32_t count, const uint32_t* v)
	{
		glProgramUniform1uiv(m_programId, GetLoc(name), count, v);
	}

	template <>
	void GLShader::Uniform<glm::uvec2>(const std::string& name, uint32_t count, const glm::uvec2* v)
	{
		glProgramUniform2uiv(m_programId, GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<glm::uvec3>(const std::string& name, uint32_t count, const glm::uvec3* v)
	{
		glProgramUniform3uiv(m_programId, GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<glm::uvec4>(const std::string& name, uint32_t count, const glm::uvec4* v)
	{
		glProgramUniform4uiv(m_programId, GetLoc(name), count, glm::value_ptr(*v));
	}

	template <>
	void GLShader::Uniform<glm::mat4>(const std::string& name, uint32_t count, const glm::mat4* v)
	{
		glProgramUniformMatrix4fv(m_programId, GetLoc(name), count, GL_FALSE, glm::value_ptr(*v));
	}
}
