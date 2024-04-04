#include "mmpch.hpp"
#include "Skin.hpp"

#include "../Files/PMXFile.hpp"
#include "Model.hpp"

#include "Core/App/Application.hpp"
#include "Core/GL/GLRenderer.hpp"
#include "Core/GL/GLVertexArray.hpp"
#include "Core/GL/GLVertexAttrib.hpp"
#include "Core/Camera/Camera.hpp"

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
		m_defaultShader = dynamic_cast<DefaultShader*>(Application::Instance().GetResourceManager().GetShader("default"));

		LoadVertices();
		LoadIndices();
		LoadMeshes();
		LoadTextures();

		m_vertexArray = std::make_unique<GLVertexArray>();
		m_vertexArray->SetVertexBuffer(*m_vertexBuffer, m_defaultShader->GetAttrib()->GetSize());
		m_vertexArray->SetVertexAttrib(*m_defaultShader->GetAttrib());
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

	GLTexture* Skin::GetTexture(int32_t idx)
	{
		return (idx < 0) ? 
			Application::Instance().GetResourceManager().GetTexture("toon00.bmp") :
			m_textures[idx].get();
	}

	void Skin::Render(GLRenderer& renderer)
	{
		renderer.Enable(GL_DEPTH_TEST);
		renderer.Enable(GL_BLEND);
		renderer.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		renderer.Enable(GL_CULL_FACE);
		renderer.CullFace(GL_FRONT);
		renderer.FrontFace(GL_CW);

		for (uint32_t i = 0; i < m_meshes.size(); ++i) {
			auto& mesh = m_meshes[i];

			// Textures
			GLTexture* albedo = GetTexture(mesh.albedoIndex);
			GLTexture* sph = GetTexture(mesh.sphIndex);
			GLTexture* toon = ((mesh.material.flags >> DefaultShader::TOON_FLAG_OFFSET) & PMXFile::TOON_SHARED_BIT) ?
				Application::Instance().GetResourceManager().GetTexture(toonTable[mesh.toonIndex]) :
				GetTexture(mesh.toonIndex);

			renderer.BeginShader(mesh.shader);
			renderer.BindTexture(*albedo, DefaultShader::ALBEDO_TEX_UNIT);
			renderer.BindTexture(*sph, DefaultShader::SPH_TEX_UNIT);
			renderer.BindTexture(*toon, DefaultShader::TOON_TEX_UNIT);
			renderer.GetShader()->Uniform("u_albedo", 1, &DefaultShader::ALBEDO_TEX_UNIT);
			//renderer.GetShader()->Uniform("u_sph", 1, &MMShader::SPH_TEX_UNIT);
			//renderer.GetShader()->Uniform("u_toon", 1, &MMShader::TOON_TEX_UNIT);

			// Materials
			dynamic_cast<DefaultShader*>(renderer.GetShader())->SetMaterial(mesh.material);

			// Shader
			const Camera* camera = renderer.GetCamera();
			if (camera != nullptr) {
				renderer.GetShader()->Uniform("u_proj", 1, &camera->GetProj());
				renderer.GetShader()->Uniform("u_view", 1, &camera->GetView());
			}

			// Draw
			bool noCull = mesh.material.flags & PMXFile::MATERIAL_NO_CULL_BIT;
			if (noCull)
				renderer.Disable(GL_CULL_FACE);
			else
				renderer.Enable(GL_CULL_FACE);

			renderer.BeginVertexArray(m_vertexArray.get());
			renderer.Draw(true, GL_TRIANGLES, mesh.elemOffset, mesh.elemCount);

			// Barrier needed because we resetted Morph in vertex shader
			renderer.Barrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}

		renderer.EndVertexArray();
		renderer.Disable(GL_CULL_FACE);
		renderer.Disable(GL_BLEND);
		renderer.Disable(GL_DEPTH_TEST);
	}

	void Skin::LoadVertices()
	{
		std::vector<DefaultShader::Attrib::Layout> vertices;

		for (const auto& pv : m_model.m_pmxFile->GetVertices()) {
			DefaultShader::Attrib::Layout v = {};
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
		m_vertexBuffer->SetData(vertices.size() * sizeof(DefaultShader::Attrib::Layout), vertices.data());
		MM_INFO("{0}: vertices loaded; count={1}", m_model.m_pmxFile->GetInfo().nameJP, vertices.size());
	}

	void Skin::LoadMeshes()
	{
		uint32_t elemOffset = 0;
		uint32_t index = 0;

		for (const auto& pm : m_model.m_pmxFile->GetMaterials()) {
			uint32_t elemCount = pm.elementCount;

			DefaultShader::MaterialLayout mat = {};
			mat.diffuse = glm::make_vec4(pm.diffuseColor);
			mat.specular = glm::vec4(glm::make_vec3(pm.specularColor), pm.specularExponent);
			mat.ambient = glm::vec4(glm::make_vec3(pm.ambientColor), 1);
			mat.edge = glm::make_vec4(pm.edgeColor);
			mat.edgeSize = pm.edgeWeight;
			//                [23:16]                [15:8]                  [7:0]
			mat.flags = (pm.toonFlag << DefaultShader::TOON_FLAG_OFFSET) | (pm.sphereMode << DefaultShader::SPH_MODE_OFFSET) | (pm.drawFlag);

			Mesh mesh = {};
			mesh.material = mat;
			mesh.elemCount = elemCount;
			mesh.elemOffset = elemOffset;
			mesh.albedoIndex = pm.textureIndex;
			mesh.sphIndex = pm.sphereIndex;
			mesh.toonIndex = pm.toonIndex;
			mesh.shader = dynamic_cast<DefaultShader*>(Application::Instance().GetResourceManager().GetShader("default"));

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
