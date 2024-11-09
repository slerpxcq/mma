#include "CorePch.hpp"
#include "Model.hpp"

#include "File/PMXFile.hpp"
#include "File/VPDFile.hpp"
#include "File/Image.hpp"

#include "Graphics/DefaultVertexLayout.hpp"
#include "Graphics/Texture2D.hpp"
#include "Material.hpp"

#include "SceneManager.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace mm
{

static Ref<VertexArray> LoadVertexArray(const PMXFile& pmx)
{
	/* Load indices */
	Ref<IndexBuffer> ib =
		MakeRef<IndexBuffer>(pmx.GetFaces().data(),
							 Graphics::IndexType::UINT,
							 // Graphics::ToIndexType(pmx.header.vertexIndexSize),
							 pmx.GetFaces().size() * 3);

	/* Load vertices */
	DynArray<DefaultVertexLayout::Struct> m_vertices{ pmx.GetVertices().size()};
	for (i32 i = 0; i < m_vertices.size(); ++i) {
		auto& v = m_vertices[i];
		const auto& pv = pmx.GetVertices()[i];

		v.position = glm::make_vec3(pv.position);
		v.normal = glm::make_vec3(pv.normal);
		v.texCoord = glm::make_vec2(pv.uv);

		switch (static_cast<PMXFile::BlendingType>(pv.blendingType)) {
		case PMXFile::BlendingType::BDEF1:
			v.m_bones[0] = pv.blending.bdef1.boneIndex;
			v.m_bones[1] = v.m_bones[2] = v.m_bones[3] = -1;
			v.weights[0] = 1.f;
			break;
		case PMXFile::BlendingType::BDEF2:
			v.m_bones[0] = pv.blending.bdef2.boneIndices[0];
			v.m_bones[1] = pv.blending.bdef2.boneIndices[1];
			v.m_bones[2] = v.m_bones[3] = -1;
			v.weights[0] = pv.blending.bdef2.weight;
			v.weights[1] = 1.f - pv.blending.bdef2.weight;
			break;
		case PMXFile::BlendingType::BDEF4:
			for (i32 j = 0; j < 4; ++j)
				v.m_bones[j] = pv.blending.bdef4.boneIndices[j];
			for (i32 j = 0; j < 3; ++j)
				v.weights[j] = pv.blending.bdef4.weights[j];
			break;
		case PMXFile::BlendingType::SDEF:
			v.m_bones[0] = pv.blending.sdef.boneIndices[0];
			v.m_bones[1] = pv.blending.sdef.boneIndices[1];
			v.m_bones[2] = -1; v.m_bones[3] = 0; /* SDEF mark */
			v.weights[0] = pv.blending.sdef.weight;
			v.weights[1] = 1.f - pv.blending.sdef.weight;
			v.sdef_c = glm::make_vec3(pv.blending.sdef.c);
			v.sdef_r0 = glm::make_vec3(pv.blending.sdef.r0);
			v.sdef_r1 = glm::make_vec3(pv.blending.sdef.r1);
			break;
		}
	}

	Ref<VertexBuffer> vb =
		MakeRef<VertexBuffer>(m_vertices.data(),
							  DefaultVertexLayout::instance,
							  m_vertices.size());

	return MakeRef<VertexArray>(vb, ib);
}

void Model::OnUpdate(f32)
{
	UpdateSkinningBuffer();
}

Ref<Model> Model::Load(const PMXFile& pmx)
{
	auto model = MakeRef<Model>(pmx.GetInfo().nameJP);

	/* Load vertices and indices */
	auto va = LoadVertexArray(pmx);

	/* Load mesh */
	model->m_mesh = MakeRef<Mesh>(model->GetName(), va);

	/* Load textures */
	DynArray<Ref<Texture>> textures{};
	textures.reserve(pmx.GetTextures().size());
	for (const auto& pt : pmx.GetTextures()) {
		String path = pmx.GetPath().parent_path().string() + '/' + pt.name;
		auto img = Image(path);
		auto tex = MakeRef<Texture2D>(img.GetWidth(), img.GetHeight(),
									  Graphics::TexFormat::RGBA8);
		tex->SetSubImage(img.GetPixels(), 
						 Graphics::PixelType::UBYTE,
						 Graphics::PixelFormat::RGBA,
						 img.GetWidth(), img.GetHeight());
		textures.push_back(std::move(tex));
	}

	/* Load sub-meshes */
	u32 offset{ 0 };
	auto mesh = model->m_mesh;
	for (const auto& pm : pmx.GetMaterials()) {
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

	/* Load bones */
	auto& bones = model->m_bones;
	bones.reserve(pmx.GetBones().size());
	i32 index{0};
	for (const auto& pb : pmx.GetBones()) {
		Transform bindWorld = { glm::make_vec3(pb.position), glm::identity<Quat>() };
		Transform bindLocal = { pb.parentIndex < 0 ?
			glm::make_vec3(pb.position) :
			glm::make_vec3(pb.position) - glm::make_vec3(pmx.GetBones()[pb.parentIndex].position),
			glm::identity<Quat>() };
		auto bone = MakeRef<Bone>(pb.nameJP, pb.parentIndex, bindLocal, bindWorld);
		model->m_boneNameIndexMap.insert({ pb.nameJP, index });
		bones.push_back(std::move(bone));
		++index;
	}
	model->m_skinningBuffer = MakeRef<ShaderStroageBuffer>();
	model->m_skinningBuffer->SetStorage(nullptr,
										bones.size() * sizeof(Mat4),
										Graphics::BufferFlags::MAP_WRITE_BIT);
	model->m_skinningBuffer->SetBindBase(0);

	return std::move(model);
}

void Model::AttachTo(Node& node)
{
	SceneObject::AttachTo(node);
	for (auto& b : m_bones) {
		if (b->GetParentIndex() < 0) {
			node.AttachObject(b);
			node.SetWorldTransform(b->GetBindWorld());
		} else {
			auto& boneNode = m_bones[b->GetParentIndex()]->GetNode()->AddChild(b->GetName());
			boneNode.AttachObject(b);
			boneNode.SetWorldTransform(b->GetBindWorld());
		}
	}
}

void Model::LoadPose(const VPDFile& vpd)
{
	auto& pose = vpd.GetPose();
	for (auto t : pose) {
		auto it = m_boneNameIndexMap.find(t.first);
		if (it != m_boneNameIndexMap.end()) {
			auto& bone = m_bones[it->second];
			bone->GetNode()->SetLocalTransform(t.second * bone->GetBindLocal());
		}
	}
}

void Model::UpdateSkinningBuffer()
{
	Mat4* ptr = reinterpret_cast<Mat4*>(m_skinningBuffer->Map(Graphics::BufferAccess::WRITE));
	for (auto& b : m_bones) {
		*ptr++ = b->GetNode()->GetWorldMatrix() * b->GetBindWorldInv().ToMat4();
	}
	m_skinningBuffer->Unmap();
}

}

