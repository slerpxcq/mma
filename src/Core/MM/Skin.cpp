#include "mmpch.hpp"
#include "Skin.hpp"

#include "Files/PMXFile.hpp"
#include "Model.hpp"

#include "Core/Application.hpp"
#include "Core/GL/GLRenderer.hpp"
#include "Core/GL/GLVertexArray.hpp"
#include "Core/GL/GLVertexAttrib.hpp"
#include "Core/Camera.hpp"

namespace mm
{
	Skin::Skin(Model& model) :
		m_model(model)
	{
		LoadVertices();
		LoadIndices();
		LoadMeshes();
		LoadTextures();

		m_vertexArray = std::make_unique<GLVertexArray>();
		m_vertexArray->SetVertexBuffer(*m_vertexBuffer, MMShader::s_vertexAttrib->GetSize());
		m_vertexArray->SetVertexAttrib(*MMShader::s_vertexAttrib);
		m_vertexArray->SetElemBuffer(*m_elemBuffer);
		m_vertexArray->SetElemType(GL_UNSIGNED_INT);
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
			*Application::Instance().GetToons()[0] :
			*m_textures[idx];
	}

	void Skin::Render(GLRenderer& renderer)
	{
		renderer.Begin(GL_DEPTH_TEST);
		renderer.Begin(GL_BLEND);
		renderer.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (uint32_t i = 0; i < m_meshes.size(); ++i) {
			const auto& mesh = m_meshes[i];

			// Textures
			GLTexture& albedo = GetTexture(mesh.albedoIndex);
			GLTexture& sph = GetTexture(mesh.sphIndex);
			GLTexture& toon = (mesh.material.flags & TOON_FLAG_BIT) ?
				*Application::Instance().GetToons()[mesh.toonIndex + 1] :
				GetTexture(mesh.toonIndex);

			renderer.UseShader(*mesh.shader);
			renderer.BindTexture(albedo, MMShader::ALBEDO_TEX_UNIT);
			renderer.BindTexture(sph, MMShader::SPH_TEX_UNIT);
			renderer.BindTexture(toon, MMShader::TOON_TEX_UNIT);
			renderer.GetShader()->Uniform("u_albedo", 1, &MMShader::ALBEDO_TEX_UNIT);
			//renderer.GetShader()->Uniform("u_sph", 1, &MMShader::SPH_TEX_UNIT);
			//renderer.GetShader()->Uniform("u_toon", 1, &MMShader::TOON_TEX_UNIT);

			// Materials
			dynamic_cast<MMShader*>(renderer.GetShader())->SetMaterial(mesh.material);

			// Shader
			const Camera* camera = renderer.GetCamera();
			if (camera != nullptr) {
				renderer.GetShader()->Uniform("u_proj", 1, &camera->GetProj());
				renderer.GetShader()->Uniform("u_view", 1, &camera->GetView());
			}

			renderer.Draw(*m_vertexArray, true, GL_TRIANGLES, mesh.elemOffset, mesh.elemCount);

			// Barrier needed because we resetted Morph in vertex shader
			renderer.Barrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}

		renderer.End(GL_BLEND);
		renderer.End(GL_DEPTH_TEST);
	}

	void Skin::LoadVertices()
	{
		std::vector<MMShader::VertexAttrib::Layout> vertices;

		for (const auto& pv : m_model.m_pmxFile->GetVertices()) {
			MMShader::VertexAttrib::Layout v = {};
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
				// TODO: use proper SDEF data
				v.bones[0] = pv.blending.sdef.boneIndices[0];
				v.bones[1] = pv.blending.sdef.boneIndices[1];
				v.weights[0] = pv.blending.sdef.weight;
				v.weights[1] = 1.f - v.weights[0];
				break;
			}

			vertices.push_back(std::move(v));
		}

		m_vertexCount = vertices.size();
		m_vertexBuffer = std::make_unique<GLBuffer>(GL_ARRAY_BUFFER);
		m_vertexBuffer->SetData(vertices.size() * sizeof(MMShader::VertexAttrib::Layout), vertices.data());
		MM_INFO("{0}: vertices loaded; count={1}", m_model.m_pmxFile->GetInfo().nameJP, vertices.size());
	}

	void Skin::LoadMeshes()
	{
		uint32_t elemOffset = 0;
		uint32_t index = 0;

		for (const auto& pm : m_model.m_pmxFile->GetMaterials()) {
			uint32_t elemCount = pm.elementCount;

			MMShader::MaterialLayout mat = {};
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
			mesh.shader = m_model.s_meshShader.get();

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
			m_textures.push_back(std::make_unique<GLTexture>(texPath.u8string().c_str(), GL_TEXTURE_2D));
		}
	}
}