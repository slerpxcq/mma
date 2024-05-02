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
		m_name = name;
		MM_INFO("{0}: Effect: {1}", __FUNCTION__, name);

		auto techniques = effect["techniques"];
		MM_ASSERT(techniques.IsSequence());

		for (const auto& technique : techniques) {
			std::string name = technique["name"].as<std::string>();
			Technique tec = {};
			tec.name = name;
			MM_INFO("{0}:   Technique: {1}", __FUNCTION__, name);

			if (technique["textures"].IsDefined()) {
				for (const auto& texture : technique["textures"]) {
					std::string name = texture["name"].as<std::string>();
				}
			}

			for (const auto& pass : technique["passes"]) {
				std::string name = pass["name"].as<std::string>();
				std::string vertexShaderPath = pass["vertex_shader"].as<std::string>();
				std::string fragmentShaderPath = pass["fragment_shader"].as<std::string>();

				MM_INFO("{0}:     Pass: {1}", __FUNCTION__, name);
				MM_INFO("{0}:     Vertex shader: {1}", __FUNCTION__, vertexShaderPath);
				MM_INFO("{0}:     Fragment shader: {1}", __FUNCTION__, fragmentShaderPath);

				Pass p = {};
				p.name = name;

				if (pass["blend"].IsDefined()) {
					bool blend = pass["blend"].as<bool>();
					p.blend = blend;
					if (blend) {
						if (!pass["blend_src"].IsDefined()) {
							throw EffectParseError("blend_src not defined");
						}
						std::string blendSrc = pass["blend_src"].as<std::string>();
						if (blendSrc == "SrcAlpha") {
							p.blendSrc = GL_SRC_ALPHA;
						}
						else {
							throw EffectParseError("Unknown blend_src value");
						}
						if (!pass["blend_dst"].IsDefined()) {
							throw EffectParseError("Unknown blend_dst value");
						}
						std::string blendDst = pass["blend_dst"].as<std::string>();
						if (blendDst == "OneMinusSrcAlpha") {
							p.blendDst = GL_ONE_MINUS_SRC_ALPHA;
						}
						else {
							throw EffectParseError("Unknown blend_dst value");
						}
					}
				}

				if (pass["depth_test"].IsDefined()) {
					bool depthTest = pass["depth_test"].as<bool>();
					p.depthTest = depthTest;
				}

				if (pass["cull_face"].IsDefined()) {
					bool cullFace = pass["cull_face"].as<bool>();
					p.cullFace = cullFace;
					if (!pass["front_face"].IsDefined()) {
						throw EffectParseError("front_face undefined");
					}
					else {
						std::string frontFace = pass["front_face"].as<std::string>();
						if (frontFace == "CW") {
							p.frontFace = GL_CW;
						}
						else if (frontFace == "CCW") {
							p.frontFace = GL_CCW;
						}
						else {
							throw EffectParseError("Unknown front_face value");
						}
					}
				}

				if (pass["uniforms"].IsDefined()) {
					for (auto& uniform : pass["uniforms"]) {
						std::string name = uniform["name"].as<std::string>();
						std::string type = uniform["type"].as<std::string>();
						Uniform u = {};
						u.name = name;
						if (type == "int") {
							u.type = Uniform::TYPE_INT;
							u.value = uniform["value"].as<int32_t>();
							//MM_INFO("{0}:     Uniform: name={1}, type={2}, value={3}", __FUNCTION__, name, "int", std::any_cast<int32_t>(u.value));
						}
						else if (type == "float") {
							u.type = Uniform::TYPE_FLOAT;
							u.value = uniform["value"].as<float>();
							//MM_INFO("{0}:     Uniform: name={1}, type={2}, value={3}", __FUNCTION__, name, "int", std::any_cast<int32_t>(u.value));
						}
						else {
							throw EffectParseError("Unknown uniform type");
						}
						p.uniforms.push_back(std::move(u));
					}
				}

				std::unique_ptr<GLShader> shader = std::make_unique<GLShader>();
				shader->Compile(vertexShaderPath, GLShader::VERTEX);
				shader->Compile(fragmentShaderPath, GLShader::FRAGMENT);
				shader->Link();

				p.program = shader.get();
				/* Should be unique name! */
				/* Otherwise it causes the shader with same name being deleted */
				ResourceManager::Instance().LoadShader(tec.name + "_" + name, std::move(shader));

				tec.passes.push_back(std::move(p));
			}
			m_techniques.insert({ name, std::move(tec) });
		}
	}

}
