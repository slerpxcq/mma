#pragma once

namespace mm
{
	class GLShader
	{
		static constexpr uint32_t MAX_SHADER_COUNT = 4;
	public:
		enum Type {
			VERTEX = 0,
			FRAGMENT,
			// GEOMETRY,
			// COMPUTE
			// OTHER TYPES...
		};
	public:
		GLShader();
		GLShader(const GLShader&) = delete;
		GLShader(GLShader&&) = default;
		virtual ~GLShader();

		void Compile(const std::filesystem::path& path, Type type);
		void Link();
		void Use() const;

		uint32_t GetId() const { return m_programId; }

		template <typename T>
		void Uniform(const std::string& name, const T& v) { Uniform(name, 1, &v); }

		template <typename T>
		void Uniform(const std::string& name, uint32_t count, const T* v) {
			int32_t loc = GetLoc(name);
			if (loc < 0)
				return;
			UniformHelper(loc, count, v);
		}

	private:
		template <typename T>
		void UniformHelper(int32_t loc, uint32_t count, const T* v);

		uint32_t GLShaderType(uint32_t type);
		uint32_t GetLoc(const std::string& name);
		void DetachAllShaders();
		void DeleteAllShaders();
		void AttachAllShaders();

	private:
		uint32_t m_programId;
		uint32_t m_shaderIds[MAX_SHADER_COUNT];
		std::unordered_map<std::string, uint32_t> m_locCache;
	};
}


