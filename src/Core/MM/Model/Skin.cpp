#include "mmpch.hpp"
#include "Skin.hpp"

#include "../Files/PMXFile.hpp"
#include "Model.hpp"

#include "Core/App/Application.hpp"
#include "Core/GL/GLRenderer.hpp"
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
		m_model(model)
	{
		GLPass pass;
		pass.blend = true;
		pass.cullFace = true;
		pass.depthTest = true;
		pass.shader = ResourceManager::s_instance.GetShader("default");
		m_defaultEffect.push_back(pass);

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
			*ResourceManager::s_instance.GetTexture("toon00.bmp") :
			*m_textures[idx];
	}

	GLTexture& Skin::GetToon(const Mesh& mesh)
	{
		return ((mesh.material.flags >> 8) & PMXFile::TOON_SHARED_BIT) ?
			*ResourceManager::s_instance.GetTexture(toonTable[mesh.toonIndex]) :
			GetTexture(mesh.toonIndex);
	}

	void Skin::Render(GLRenderer& renderer)
	{
		for (uint32_t i = 0; i < m_meshes.size(); ++i) {
			auto& mesh = m_meshes[i];

			for (uint32_t pass = 0; pass < mesh.effect->size(); ++pass) {
				renderer.BeginPass((*mesh.effect)[pass]);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				GLTexture& albedo = GetTexture(mesh.albedoIndex);
				GLTexture& sph = GetTexture(mesh.sphIndex);
				GLTexture& toon = GetToon(mesh);
				albedo.Bind(0);
				sph.Bind(1);
				toon.Bind(2);

				GLTexture& skybox = *ResourceManager::s_instance.GetTexture("skybox");
				skybox.Bind(3);

				static int32_t ZERO = 0;
				static int32_t ONE = 1;
				static int32_t TWO = 2;
				static int32_t THREE = 3;

				renderer.Uniform("u_albedo", 1, &ZERO);
				renderer.Uniform("u_sph", 1, &ONE);
				renderer.Uniform("u_toon", 1, &TWO);
				renderer.Uniform("u_skybox", 1, &THREE);

				renderer.SetMaterial(mesh.material);

				static glm::vec3 lightDir(-.5, -1, .5);
				static glm::vec3 lightColor(.6, .6, .6);
				renderer.Uniform("u_lightDir", 1, &lightDir);
				renderer.Uniform("u_lightColor", 1, &lightColor);

				if (mesh.material.flags & PMXFile::MATERIAL_NO_CULL_BIT) {
					glDisable(GL_CULL_FACE);
				}
				else {
					glEnable(GL_CULL_FACE);
					glCullFace(GL_FRONT);
					glFrontFace(GL_CW);
				}
				
				m_vertexArray->Bind();
				m_vertexArray->DrawElem(GL_TRIANGLES, mesh.elemOffset, mesh.elemCount);
				// Barrier needed because we resetted Morph in vertex shader
				glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
				renderer.EndPass();
			}
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
				// SDEF indication
				v.bones[3] = 0;
				v.weights[0] = pv.blending.sdef.weight;
				v.weights[1] = 1.f - v.weights[0];
				v.sdef_c = glm::make_vec3(pv.blending.sdef.c);
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

			MaterialUBOLayout mat = {};
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
