#include "mmpch.hpp"
#include "Effect.hpp"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

#include "Core/GL/GLShader.hpp"
#include "Core/ResourceManager/ResourceManager.hpp"

namespace mm
{
	Effect::Effect(const std::filesystem::path& path)
	{
		YAML::Node root;
		try {
			root = YAML::LoadFile(path.u8string());
		}
		catch (const YAML::BadFile& e) {
			MM_ERROR("{0}: Failed to open effect file", __FUNCTION__);
			MM_ASSERT(0);
		} 
		catch (const YAML::ParserException& e) {
			MM_ERROR("{0}: Failed to parse effect file", __FUNCTION__);
			MM_ASSERT(0);
		}

		auto effect = root["effect"];

		std::string name = effect["name"].as<std::string>();
		MM_INFO("{0}: Effect: {1}", __FUNCTION__, name);

		auto techniques = effect["techniques"];
		MM_ASSERT(techniques.IsSequence());

		for (auto technique : techniques) {
			std::string name = technique["name"].as<std::string>();
			Technique tec = {};
			tec.name = name;
			MM_INFO("{0}:   Technique: {1}", __FUNCTION__, name);

			for (auto pass : technique["passes"]) {
				std::string name = pass["name"].as<std::string>();
				std::string vertexShaderPath = pass["vertex_shader"].as<std::string>();
				std::string fragmentShaderPath = pass["fragment_shader"].as<std::string>();

				MM_INFO("{0}:     Pass: {1}", __FUNCTION__, name);
				MM_INFO("{0}:     Vertex shader: {1}", __FUNCTION__, vertexShaderPath);
				MM_INFO("{0}:     Fragment shader: {1}", __FUNCTION__, fragmentShaderPath);

				if (pass["blend"].IsDefined()) {
					bool blend = pass["blend"].as<bool>();
					std::string blendSrc = pass["blend_src"].as<std::string>();
					if (blendSrc == "SrcAlpha") {
					}
					else {
						throw EffectParseError("Unknown blend_src value");
					}
					std::string blendDst = pass["blend_dst"].as<std::string>();
					if (blendSrc == "OneMinusSrcAlpha") {
					}
					else {
						throw EffectParseError("Unknown blend_dst value");
					}
				}

				if (pass["depth_test"].IsDefined()) {
					bool depthTest = pass["depth_test"].as<bool>();
				}

				std::unique_ptr<GLShader> shader = std::make_unique<GLShader>();
				shader->Compile(vertexShaderPath, GLShader::VERTEX);
				shader->Compile(fragmentShaderPath, GLShader::FRAGMENT);
				shader->Link();

				Pass p = {};
				p.name = name;
				p.program = shader.get();
				ResourceManager::Instance().LoadShader(name, std::move(shader));

				tec.passes.push_back(std::move(p));
			}
			m_techniques.insert({ name, std::move(tec) });
		}

		for (const auto& tec : m_techniques) {
		}
	}
}
