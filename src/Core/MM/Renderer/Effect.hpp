#pragma once

#include "Core/GL/GLTexture.hpp"
#include "Core/GL/GLFrameBuffer.hpp"

namespace mm
{
	struct EffectParseError : public std::runtime_error 
	{
		EffectParseError(const char* what) : std::runtime_error(what) {}
	};

	class GLShader;

	class Effect
	{
	public:
		struct Uniform {
			enum {
				TYPE_INT,
				TYPE_FLOAT
			};
			std::string name;
			int32_t location;
			uint8_t type;
			std::any value;
		};

		struct Pass {
			std::string name;
			GLShader* program = nullptr;
			bool depthTest = false;
			bool blend = false;
			uint32_t blendSrc = 0;
			uint32_t blendDst = 0;
			bool cullFace = false;
			uint32_t frontFace = 0;
			std::vector<Uniform> uniforms;
		};

		struct Technique { 
			std::string name;
			std::vector<Pass> passes;
		};

	public:
		Effect(const std::filesystem::path& path);
		std::string GetName() { return m_name; };
		Technique* GetTechnique(const std::string& name) { 
			auto it = m_techniques.find(name);
			if (it != m_techniques.end())
				return &(it->second);
			else
				return nullptr;
		}

	private:
		std::string m_name;
		std::unordered_map<std::string, Technique> m_techniques;
		std::unordered_map<std::string, std::unique_ptr<GLTexture>> m_textures;
		std::unordered_map<std::string, std::unique_ptr<GLFrameBuffer>> m_framebuffers;
	};
}


