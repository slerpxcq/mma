#pragma once

#include "../Files/PMXFile.hpp"
#include "Core/MM/Math/Transform.hpp" 

#include "Core/GL/GLVertexArray.hpp"

namespace mm
{
	class Model;
	class Renderer;
	class GLShader;

	class Morph
	{
	public:
		static constexpr uint32_t MORPH_VERTEX_BASE = 2;

	public:
		using Dict = std::unordered_map<std::string, int32_t>;

	public:
		struct Target {
			uint32_t index;
		};

		// std140
		struct VertexLayout {
			glm::vec3 pos;
			float     pad0;
			glm::vec2 uv;
			glm::vec2 pad1;
		}; 

		struct VertexTarget : public Target {
			std::unique_ptr<GLVertexArray> vertexArray;
			std::unique_ptr<GLBuffer> offsetBuffer;
			uint32_t offsetCount;
		};

		struct MaterialTarget : public Target {
			struct Offset {
				int32_t index;
				glm::vec4 diffuse;
				glm::vec4 specular;
				glm::vec4 ambient;
				glm::vec4 edge;
				glm::vec4 texTint;
				glm::vec4 sphTint;
				glm::vec4 toonTint;
				float edgeSize;
				uint32_t mode;
			};

			std::vector<Offset> offsets;
		};

		struct BoneTarget : public Target {
			struct Offset {
				int32_t boneIndex;
				Transform locXform;
			};
			std::vector<Offset> offsets;
		};

	public:
		Morph(Model& model);
		std::vector<float>& GetWeights() { return m_weights; }
		const std::vector<VertexTarget>& GetVertexTargets() const { return m_vertexTargets; }
		const std::vector<MaterialTarget>& GetMaterialTargets() const { return m_materialTargets; }
		const Dict& GetDict() { return m_dict;  }

		void Render(Renderer& renderer) const;

	private:
		void LoadTargets();
		VertexTarget LoadVertexTarget(const PMXFile::Morph& target);
		MaterialTarget LoadMaterialTarget(const PMXFile::Morph& target);
		BoneTarget LoadBoneTarget(const PMXFile::Morph& target);

	private:
		Model& m_model;
		Dict m_dict;
		std::vector<float> m_weights;
		
		std::vector<VertexTarget> m_vertexTargets;
		std::vector<MaterialTarget> m_materialTargets;
		std::vector<BoneTarget> m_boneTargets;

		GLShader* m_morphShader;
	};
}


