#include "CorePch.hpp"
#include "Model.hpp"

#include "File/PMXFile.hpp"
#include "File/Image.hpp"

#include "Graphics/DefaultVertexLayout.hpp"
#include "Graphics/Texture2D.hpp"
#include "Material.hpp"

#include "SceneManager.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace mm
{

static Ref<VertexArray> LoadVertexArrayPMX(const PMXFile& pmx)
{
	/* Load indices */
	Ref<IndexBuffer> ib =
		MakeRef<IndexBuffer>(pmx.faces.data(),
							 Graphics::IndexType::UINT,
							 // Graphics::ToIndexType(pmx.header.vertexIndexSize),
							 pmx.faces.size() * 3);

	/* Load vertices */
	DynArray<DefaultVertexLayout::Struct> vertices{ pmx.vertices.size() };
	for (i32 i = 0; i < vertices.size(); ++i) {
		auto& v = vertices[i];
		const auto& pv = pmx.vertices[i];

		v.position = glm::make_vec3(pv.position);
		v.normal = glm::make_vec3(pv.normal);
		v.texCoord = glm::make_vec2(pv.uv);

		switch (static_cast<PMXFile::BlendingType>(pv.blendingType)) {
		case PMXFile::BlendingType::BDEF1:
			v.bones[0] = pv.blending.bdef1.boneIndex;
			v.bones[1] = v.bones[2] = v.bones[3] = -1;
			v.weights[0] = 1.f;
			break;
		case PMXFile::BlendingType::BDEF2:
			v.bones[0] = pv.blending.bdef2.boneIndices[0];
			v.bones[1] = pv.blending.bdef2.boneIndices[1];
			v.bones[2] = v.bones[3] = -1;
			v.weights[0] = pv.blending.bdef2.weight;
			v.weights[1] = 1.f - pv.blending.bdef2.weight;
			break;
		case PMXFile::BlendingType::BDEF4:
			for (i32 j = 0; j < 4; ++j)
				v.bones[j] = pv.blending.bdef4.boneIndices[j];
			for (i32 j = 0; j < 3; ++j)
				v.weights[j] = pv.blending.bdef4.weights[j];
			break;
		case PMXFile::BlendingType::SDEF:
			v.bones[0] = pv.blending.sdef.boneIndices[0];
			v.bones[1] = pv.blending.sdef.boneIndices[1];
			v.bones[2] = -1; v.bones[3] = 0; /* SDEF mark */
			v.weights[0] = pv.blending.sdef.weight;
			v.weights[1] = 1.f - pv.blending.sdef.weight;
			v.sdef_c = glm::make_vec3(pv.blending.sdef.c);
			v.sdef_r0 = glm::make_vec3(pv.blending.sdef.r0);
			v.sdef_r1 = glm::make_vec3(pv.blending.sdef.r1);
			break;
		}
	}

	Ref<VertexBuffer> vb =
		MakeRef<VertexBuffer>(vertices.data(),
							  DefaultVertexLayout::instance,
							  vertices.size());

	return MakeRef<VertexArray>(vb, ib);
}

Ref<Model> Model::LoadFromPMX(const PMXFile& pmx)
{
	auto model = MakeRef<Model>(pmx.info.nameJP);

	/* Load vertices and indices */
	auto va = LoadVertexArrayPMX(pmx);

	/* Load mesh */
	model->m_mesh = MakeRef<Mesh>(model->GetName(), va);

	/* Load textures */
	DynArray<Ref<Texture>> textures{};
	textures.reserve(pmx.textures.size());
	for (const auto& pt : pmx.textures) {
		String path = pmx.GetPath().parent_path().string() + '/' + pt.name;
		auto img = Image::Load(path);
		auto tex = MakeRef<Texture2D>(img->GetWidth(), img->GetHeight(),
									  Graphics::TexFormat::RGBA8);
		tex->SetSubImage(img->GetPixels(), 
						 Graphics::PixelType::UBYTE,
						 Graphics::PixelFormat::RGBA,
						 img->GetWidth(), img->GetHeight());
		textures.push_back(std::move(tex));
	}

	/* Load sub-meshes */
	u32 offset{ 0 };
	auto mesh = model->m_mesh;
	for (const auto& pm : pmx.materials) {
		auto tex = pm.textureIndex < 0 ? 
			GetDefaultTextures()[0] :
			textures[pm.textureIndex];
		u32 flags{};
		if (pm.drawFlag & static_cast<u8>(PMXFile::MaterialFlag::NO_CULL_BIT)) {
			flags |= static_cast<u32>(Material::Flags::NO_CULL_BIT);
		}
		auto material = Ref<Material>(new Material{ pm.nameJP,
									  { std::make_pair(Material::MapType::ALBEDO, tex) },
									  GetDefaultProgram(),
									  flags });
		mesh->AddSubMesh(pm.nameJP, material, offset, pm.elementCount);
		offset += pm.elementCount;
	}

	return std::move(model);
}

}

