#include "mmpch.hpp"
#include "Skin.hpp"

#include "PMXFile.hpp"
#include "Model.hpp"

namespace mm
{
	void Skin::Attrib::Set() const
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Layout), (void*)offsetof(Layout, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Layout), (void*)offsetof(Layout, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Layout), (void*)offsetof(Layout, uv));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_INT, GL_FALSE, sizeof(Layout), (void*)offsetof(Layout, bones));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Layout), (void*)offsetof(Layout, weights));
	}

	Skin::Skin(Model& model) :
		m_model(model)
	{
		m_vertexAttrib = std::make_unique<Attrib>();
		LoadVertices();
		LoadMeshes();
		LoadTextures();
	}

	void Skin::LoadVertices()
	{
		m_vbo = std::make_unique<GLBuffer>(GL_ARRAY_BUFFER);

		std::vector<Attrib::Layout> vertices;

		for (const auto& pv : m_model.m_pmxFile->GetVertices()) {
			Attrib::Layout v = {};
			v.position = glm::make_vec3(pv.position),
			v.normal = glm::make_vec3(pv.normal),
			v.uv = glm::make_vec3(pv.uv),
			v.bones = glm::ivec4(-1),
			v.weights = glm::ivec3(0);

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
			}
			break;
			case PMXFile::VERTEX_SDEF:
				v.bones[0] = pv.blending.sdef.boneIndices[0];
				v.bones[1] = pv.blending.sdef.boneIndices[1];
				v.weights[0] = pv.blending.sdef.weight;
				v.weights[1] = 1.f - v.weights[0];
				break;
			}

			vertices.push_back(v);
		}

		m_vbo->Data(vertices.size() * sizeof(Attrib::Layout), vertices.data());
		MM_INFO("{0}: Vertices loaded", m_model.m_pmxFile->GetInfo().nameJP);
	}

	void Skin::LoadMeshes()
	{
		uint32_t faceOffset = 0;

		for (const auto& pm : m_model.m_pmxFile->GetMaterials()) {
			uint32_t faceCount = pm.faceCount / 3;

			Material mat = {};
			mat.diffuse = glm::make_vec4(pm.diffuseColor);
			mat.specular = glm::vec4(glm::make_vec3(pm.specularColor), pm.specularExponent);
			mat.ambient = glm::vec4(glm::make_vec3(pm.ambientColor), 1);
			mat.edge = glm::make_vec4(pm.edgeColor);
			mat.edgeSize = pm.edgeWeight;
			//                [23:16]                [15:8]                  [7:0]
			mat.flags = (pm.toonFlag << 16) | (pm.sphereMode << 8) | (pm.drawFlag);
			mat.albedoIndex = pm.textureIndex;
			mat.sphIndex = pm.sphereIndex;
			mat.toonIndex = pm.toonIndex;

			Mesh mesh = {};
			mesh.material = mat;
			mesh.faceCount = faceCount;
			mesh.vertexArray.Attrib(m_vertexAttrib.get());
			mesh.elemBuf = std::make_unique<GLBuffer>(GL_ELEMENT_ARRAY_BUFFER);
			mesh.elemBuf->Data(
				3 * faceCount * sizeof(int32_t),
				(void*)&m_model.m_pmxFile->GetFaces()[faceOffset]);

			m_meshes.push_back(std::move(mesh));
			faceOffset += faceCount;
		}
		MM_INFO("{0}: Meshes loaded", m_model.m_pmxFile->GetInfo().nameJP);
	}

	void Skin::LoadTextures()
	{
		for (const auto& px : m_model.m_pmxFile->GetTextures()) {
			std::filesystem::path texPath = m_model.m_pmxFile->GetPath();
			texPath.remove_filename();
			texPath += px.name;
			m_textures.emplace_back(texPath.u8string().c_str(), GL_TEXTURE_2D);
		}
	}
}
