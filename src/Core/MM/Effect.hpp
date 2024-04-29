#pragma once

#include "Core/GL/GLShader.hpp"

namespace mm
{
	struct EffectParseError : public std::runtime_error 
	{
		EffectParseError(const char* what) : std::runtime_error(what) {}
	};

	class Effect
	{
	public:
		struct Pass {
			std::string name;
			GLShader* program = nullptr;
			bool depthTest = false;
			bool blend = false;
			uint32_t blendFuncSrc;
			uint32_t blendFuncDst;
		};

		struct Technique { 
			std::string name;
			std::vector<Pass> passes;
		};

	public:
		Effect(const std::filesystem::path& path);

	private:
		std::map<std::string, Technique> m_techniques;
	};
}


