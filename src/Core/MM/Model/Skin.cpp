#include "mmpch.hpp"
#include "Skin.hpp"

#include "../Files/PMXFile.hpp"
#include "Model.hpp"

#include "Core/App/Application.hpp"
#include "Core/MM/Renderer/Renderer.hpp"
#include "Core/GL/GLVertexArray.hpp"
#include "Core/GL/GLVertexAttrib.hpp"
#include "Core/MM/Camera/Camera.hpp"

#include "SkinVertex.hpp"

static constexpr const char* toonTable[] = {
	"toon01.bmp", "toon02.bmp",
	"toon03.bmp", "toon04.bmp",
	"toon05.bmp", "toon06.bmp",
	"toon07.bmp", "toon08.bmp",
	"toon09.bmp", "toon10.bmp",
};

namespace mm
{
	Skin::Skin(Model& model) :
		m_model(model),
		m_defaultEffect(*ResourceManager::Instance().GetEffect("DefaultEffect"))
	{
		LoadVertices();
		LoadIndices();
		LoadMeshes();
		LoadTextures();

		m_vertexArray = std::make_unique<GLVertexArray>();
		m_vertexArray->SetVertexBuffer(*m_vertexBuffer, sizeof(SkinVertex::Layout));
		m_vertexArray->SetVertexAttrib(SkinVertex::s_instance);
		m_vertexArray->SetElemBuffer(*m_elemBuffer);
		m_vertexArray->SetElemType(GL_UNSIGNED_INT);

		m_renderBuffer = std::make_unique<GLFrameBuffer>(glm::uvec2(1, 1));
	}

	void Skin::LoadIndices()
	{
		m_elemBuffer = std::make_unique<GLBuffer>(GL_ELEMENT_ARRAY_BUFFER);
		const auto& faces = m_model.m_pmxFile->GetFaces();
		m_elemBuffer->SetData(
			faces.size() * sizeof(PMXFile::Face),
			(void*)faces.data());
		MM_INFO("{0}: indices loaded; count={1}", m_model.m_pmxFile->GetInfo().nameJP, faces.size());
	}

	GLTexture& Skin::GetTexture(int32_t idx)
	{
		return (idx < 0) ? 
			*ResourceManager::Instance().GetTexture("toon00.bmp") :
			*m_textures[idx];
	}

	GLTexture& Skin::GetToon(const Mesh& mesh)
	{
		return ((mesh.material.flags >> 8) & PMXFile::TOON_SHARED_BIT) ?
			*ResourceManager::Instance().GetTexture(toonTable[mesh.toonIndex]) :
			GetTexture(mesh.toonIndex);
	}

	void Skin::Render(Renderer& renderer)
	{
		/* Outline pass */
		for (auto& mesh : m_meshes) {
			if (!(mesh.material.flags & PMXFile::MATERIAL_EDGE_BIT))
				continue;
			renderer.BeginEffect(mesh.effect);
			renderer.BeginTechnique("OutlineTec");
			for (auto& pass : renderer.GetActiveTechniquePasses()) {
				renderer.BeginPass(pass);
				m_vertexArray->Bind();
				m_vertexArray->DrawElem(GL_TRIANGLES, mesh.elemOffset, mesh.elemCount);
				renderer.EndPass();
			}
			renderer.EndTechnique();
			renderer.EndEffect();
		}

		/* Main pass */
		for (uint32_t meshIndex = 0; meshIndex < m_meshes.size(); ++meshIndex) {
			auto& mesh = m_meshes[meshIndex];

			renderer.BeginEffect(mesh.effect);
			renderer.BeginTechnique("MainTec");

			for (const auto& pass : renderer.GetActiveTechniquePasses()) {
				renderer.BeginPass(pass);

				if (mesh.material.flags & PMXFile::MATERIAL_NO_CULL_BIT)
					glDisable(GL_CULL_FACE);

				/* These should be set in BeginPass */
				GLTexture& albedo = GetTexture(mesh.albedoIndex);
				GLTexture& sph = GetTexture(mesh.sphIndex);
				GLTexture& toon = GetToon(mesh);
				GLTexture& skybox = *ResourceManager::Instance().GetTexture("skybox");
				albedo.Bind(0);
				sph.Bind(1);
				toon.Bind(2);
				skybox.Bind(3);

				renderer.SetMaterial(m_model.m_materialMorphBuffer[meshIndex]);

				GLShader* shader = pass.program;
				shader->Uniform("u_albedo", 0);
				shader->Uniform("u_sph", 1);
				shader->Uniform("u_toon", 2);
				shader->Uniform("u_skybox", 3);

				shader->Uniform("u_lightDir", glm::vec3(-.5, -1, .5));
				shader->Uniform("u_lightColor", glm::vec3(.6, .6, .6));

				m_vertexArray->Bind();
				m_vertexArray->DrawElem(GL_TRIANGLES, mesh.elemOffset, mesh.elemCount);

				glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
				renderer.EndPass();
			}

			renderer.EndTechnique();
			renderer.EndEffect();
		}

		// Reset material morph offsets
		for (uint32_t meshIndex = 0; meshIndex < m_model.m_materialMorphBuffer.size(); ++meshIndex) {
			auto& offset = m_model.m_materialMorphBuffer[meshIndex];
			const auto& material = m_meshes[meshIndex].material;
			offset.ambient = material.ambient;
			offset.diffuse = material.diffuse;
			offset.specular = material.specular;
			offset.edge = material.edge;
			offset.edgeSize = material.edgeSize;
		}
	}

	void Skin::LoadVertices()
	{
		std::vector<SkinVertex::Layout> vertices;

		for (const auto& pv : m_model.m_pmxFile->GetVertices()) {
			SkinVertex::Layout v = {};
			v.position = glm::make_vec3(pv.position),
			v.normal = glm::make_vec3(pv.normal),
			v.uv = glm::make_vec2(pv.uv),
			v.bones = glm::ivec4(-1),
			v.weights = glm::vec3(0);

			switch (pv.blendingType) {
			case PMXFile::VERTEX_BDEF1:
				v.bones[0] = pv.blending.bdef1.boneIndex;
				v.weights[0] = 1.f;
				break;
			case PMXFile::VERTEX_BDEF2:
				v.bones[0] = pv.blending.bdef2.boneIndices[0];
				v.bones[1] = pv.blending.bdef2.boneIndices[1];
				v.weights[0] = pv.blending.bdef2.weight;
				v.weights[1] = 1.f - v.weights[0];
				break;
			case PMXFile::VERTEX_BDEF4:
			{
				for (uint32_t i = 0; i < 4; ++i) {
					v.bones[i] = pv.blending.bdef4.boneIndices[i];
					if (i < 3)
						v.weights[i] = pv.blending.bdef4.weights[i];
				}
				break;
			}
			case PMXFile::VERTEX_SDEF:
				v.bones[0] = pv.blending.sdef.boneIndices[0];
				v.bones[1] = pv.blending.sdef.boneIndices[1];

				/* SDEF indication */
				v.bones[3] = 0;
				v.weights[0] = pv.blending.sdef.weight;
				v.weights[1] = 1.f - v.weights[0];
				v.sdef_c = glm::make_vec3(pv.blending.sdef.c);
				v.sdef_r0 = glm::make_vec3(pv.blending.sdef.r0);
				v.sdef_r1 = glm::make_vec3(pv.blending.sdef.r1);
				break;
			}

			vertices.push_back(std::move(v));
		}

		m_vertexCount = vertices.size();
		m_vertexBuffer = std::make_unique<GLBuffer>(GL_ARRAY_BUFFER);
		m_vertexBuffer->SetData(vertices.size() * sizeof(SkinVertex::Layout), vertices.data());
		MM_INFO("{0}: vertices loaded; count={1}", m_model.m_pmxFile->GetInfo().nameJP, vertices.size());
	}

	void Skin::LoadMeshes()
	{
		uint32_t elemOffset = 0;
		uint32_t index = 0;

		for (const auto& pm : m_model.m_pmxFile->GetMaterials()) {
			uint32_t elemCount = pm.elementCount;

			MaterialLayout mat = {};
			mat.diffuse = glm::make_vec4(pm.diffuseColor);
			mat.specular = glm::vec4(glm::make_vec3(pm.specularColor), pm.specularExponent);
			mat.ambient = glm::vec4(glm::make_vec3(pm.ambientColor), 1);
			mat.edge = glm::make_vec4(pm.edgeColor);
			mat.edgeSize = pm.edgeWeight;
			//                [23:16]                [15:8]                  [7:0]
			mat.flags = (pm.toonFlag << 16) | (pm.sphereMode << 8) | (pm.drawFlag);

			Mesh mesh = {};
			mesh.material = mat;
			mesh.elemCount = elemCount;
			mesh.elemOffset = elemOffset;
			mesh.albedoIndex = pm.textureIndex;
			mesh.sphIndex = pm.sphereIndex;
			mesh.toonIndex = pm.toonIndex;
			mesh.effect = &m_defaultEffect;
			mesh.name = pm.nameJP;
			//mesh.shader = m_defaultShader;

			m_meshes.push_back(std::move(mesh));
			MM_INFO("{0}: mesh loaded; faceCount={1}, faceOffset={2}",
				pm.nameJP, elemCount / 3, elemOffset / 3);
			elemOffset += elemCount;
		}
	}

	void Skin::LoadTextures()
	{
		for (const auto& px : m_model.m_pmxFile->GetTextures()) {
			std::filesystem::path texPath = m_model.m_pmxFile->GetPath();
			texPath.remove_filename();
			texPath += px.name;
			m_textures.push_back(std::make_unique<GLTexture2D>(texPath.u8string().c_str()));
		}
	}
}
