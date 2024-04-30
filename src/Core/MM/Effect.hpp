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
			uint32_t blendSrc = 0;
			uint32_t blendDst = 0;
			bool cullFace = false;
			uint32_t frontFace = 0;
		};

		struct Technique { 
			std::string name;
			std::vector<Pass> passes;
		};

	public:
		Effect(const std::filesystem::path& path);
		std::string GetName() { return m_name; };
		void BeginTechnique(const std::string& name);
		const auto& GetActiveTechniquePasses() { return m_activeTechnique->passes; };
		void BeginPass(const Pass& pass);
		void EndPass();
		void EndTechnique();

	private:
		std::string m_name;
		std::map<std::string, Technique> m_techniques;
		Technique* m_activeTechnique = nullptr;
		Pass* m_activePass = nullptr;
		Pass m_backupState;
	};
}


