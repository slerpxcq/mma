#include "mmpch.hpp"
#include "Morph.hpp"

#include "Model.hpp"
#include "../Shaders/MorphShader.hpp"

#include "Core/GL/GLVertexArray.hpp"
#include "Core/GL/GLRenderer.hpp"
#include "Core/GL/GLBuffer.hpp"
#include "Core/GL/GLContext.hpp"

#include "Core/App/Application.hpp"

namespace mm 
{
	Morph::Morph(Model& model) :
		m_model(model)
	{
		m_morphShader = dynamic_cast<MorphShader*>(Application::Instance().GetResourceManager().GetShader("morph"));

		m_weights.resize(m_model.m_pmxFile->GetMorphs().size());
		LoadTargets();

		MM_INFO("{0}: morph loaded", m_model.m_pmxFile->GetInfo().nameJP);
	}

	void Morph::Render(GLRenderer& renderer) const
	{
		renderer.Enable(GL_RASTERIZER_DISCARD);
		renderer.BeginShader(Application::Instance().GetResourceManager().GetShader("morph"));

		for (const auto& target : m_vertexTargets) {
			renderer.GetShader()->Uniform("u_weight", 1, &m_weights[target.index]);
			renderer.BeginVertexArray(target.vertexArray.get());
			renderer.Draw(false, GL_POINTS, 0, target.offsetCount);
			renderer.Barrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}

		renderer.EndVertexArray();
		renderer.Disable(GL_RASTERIZER_DISCARD);
	}

	Morph::VertexTarget Morph::LoadVertexTarget(const PMXFile::Morph& pmxMorph)
	{
		std::vector<MorphShader::Attrib::Layout> offsets;

		for (const auto& pmxOffset : pmxMorph.offsets) {
			MorphShader::Attrib::Layout offset = {};
			if (pmxMorph.type == PMXFile::MORPH_VERTEX) {
				offset.posOffset = glm::make_vec3(pmxOffset.vertex.value);
				offset.uvOffset = glm::vec2(0);
				offset.vertexIndex = pmxOffset.vertex.vertexIndex;
			}
			else { // PMXFile::MORPH_UV
				offset.posOffset = glm::vec3(0);
				offset.uvOffset = glm::make_vec2(pmxOffset.uv.value);
				offset.vertexIndex = pmxOffset.uv.vertexIndex;
			}
			offsets.push_back(offset);
		}

		Morph::VertexTarget target = {};
		target.offsetBuffer = std::make_unique<GLBuffer>(GL_ARRAY_BUFFER);
		target.vertexArray = std::make_unique<GLVertexArray>();

		target.offsetBuffer->SetData(offsets.size() * sizeof(MorphShader::Attrib::Layout), offsets.data());
		target.vertexArray->SetVertexBuffer(*target.offsetBuffer, m_morphShader->GetAttrib()->GetSize());
		target.vertexArray->SetVertexAttrib(*m_morphShader->GetAttrib());
		target.offsetCount = offsets.size();

		MM_INFO("{0}: vertex morph loaded; count={1}", pmxMorph.nameJP, target.offsetCount);
		return target;
	}

	Morph::MaterialTarget Morph::LoadMaterialTarget(const PMXFile::Morph& pmxMorph)
	{
		MaterialTarget target = {};

		for (const auto& pmxOffset : pmxMorph.offsets) {
			MaterialTarget::Offset offset = {};
			const auto matOffset = pmxOffset.material;
			offset.diffuse = glm::make_vec4(matOffset.diffuseColor);
			offset.specular = glm::vec4(
				glm::make_vec3(matOffset.specularColor),
				matOffset.specularExponent);
			offset.ambient = glm::vec4(glm::make_vec3(matOffset.ambientColor), 0);
			offset.edge = glm::make_vec4(matOffset.edgeColor);
			offset.edgeSize = matOffset.edgeWeight;
			offset.texTint = glm::make_vec4(matOffset.textureTint);
			offset.sphTint = glm::make_vec4(matOffset.environtMapTint);
			offset.toonTint = glm::make_vec4(matOffset.toonTint);
			offset.mode = matOffset.mode;
			target.offsets.push_back(offset);
		}

		return target;
	}

	Morph::BoneTarget Morph::LoadBoneTarget(const PMXFile::Morph& pmxMorph)
	{
		BoneTarget target = {};

		for (const auto& pmxOffset : pmxMorph.offsets) {
			BoneTarget::Offset offset = {};
			offset.boneIndex = pmxOffset.bone.boneIndex;
			offset.locXform = Transform(
				glm::make_vec3(pmxOffset.bone.move),
				glm::make_quat(pmxOffset.bone.rotation));
		}

		return target;
	}

	void Morph::LoadTargets()
	{
		const auto& pmxMorphs = m_model.m_pmxFile->GetMorphs();
		for (uint32_t i = 0; i < pmxMorphs.size(); ++i) {
			switch (pmxMorphs[i].type) {
			case PMXFile::MORPH_VERTEX:	
			case PMXFile::MORPH_UV:
				m_vertexTargets.push_back(LoadVertexTarget(pmxMorphs[i]));
				m_vertexTargets.back().index = i;
				break;
			case PMXFile::MORPH_MATERIAL:
				m_materialTargets.push_back(LoadMaterialTarget(pmxMorphs[i]));
				m_materialTargets.back().index = i;
				break;
			case PMXFile::MORPH_BONE:
				m_boneTargets.push_back(LoadBoneTarget(pmxMorphs[i]));
				m_boneTargets.back().index = i;
				break;
			default:
				assert(0 && "Unsupported morph type");
			}

			m_dict.insert({ pmxMorphs[i].nameJP, i });
		}
	}
}
