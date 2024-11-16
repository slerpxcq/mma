#include "CorePch.hpp"
#include "Model.hpp"

#include "File/PMXFile.hpp"
#include "File/VPDFile.hpp"
#include "File/Image.hpp"

#include "Graphics/DefaultVertexLayout.hpp"
#include "Graphics/Texture2D.hpp"
#include "Material.hpp"

#include "SceneManager.hpp"
#include "Physics/PhysicsManager.hpp"

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
	DynArray<DefaultVertexLayout::Struct> m_vertices{ pmx.GetVertices().size() };
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

static DynArray<Ref<Texture>> LoadTextures(const PMXFile& pmx)
{
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
	return std::move(textures);
}

static void LoadSubMeshes(const PMXFile& pmx, const DynArray<Ref<Texture>>& textures, Mesh& mesh)
{
	auto renderer = GetRenderer();
	u32 offset{ 0 };
	for (const auto& pm : pmx.GetMaterials()) {
		auto tex = pm.textureIndex < 0 ? 
			renderer->GetInternalTextures()[0] :
			textures[pm.textureIndex];
		u32 flags{};
		if (pm.drawFlag & static_cast<u8>(PMXFile::MaterialFlag::NO_CULL_BIT)) {
			flags |= static_cast<u32>(Material::Flags::NO_CULL_BIT);
		}
		auto material = Ref<Material>(new Material{ pm.nameJP,
									  { MakePair(Material::MapType::ALBEDO, tex) },
									  renderer->GetDefaultProgram(),
									  flags });
		mesh.CreateSubMesh(pm.nameJP, material, offset, pm.elementCount);
		offset += pm.elementCount;
	}
}

static Collider LoadCollider(const PMXFile::Rigidbody& pr)
{
	Collider::ConstructInfo info{};
	switch (static_cast<PMXFile::RigidbodyShape>(pr.shape)) {
	case PMXFile::RigidbodyShape::BOX:
		info.type = Collider::ConstructInfo::BOX;
		info.data.box.size = glm::make_vec3(pr.size);
		break;
	case PMXFile::RigidbodyShape::CAPSULE:
		info.type = Collider::ConstructInfo::CAPSULE;
		info.data.capsule.radius = pr.size[0];
		info.data.capsule.height = pr.size[1];
		break;
	case PMXFile::RigidbodyShape::SPHERE:
		info.type = Collider::ConstructInfo::SPHERE;
		info.data.sphere.radius = pr.size[0];
		break;
	default:
		MM_CORE_UNREACHABLE();
	}
	auto collider = GetPhysicsManager()->CreateCollider(info);
	return collider;
}

static DynArray<Rigidbody*> LoadRigidbodies(const PMXFile& pmx, const DynArray<Bone*>& bones)
{
	auto pm = GetPhysicsManager();
	auto sm = GetSceneManager();
	DynArray<Rigidbody*> result;
	result.reserve(pmx.GetRigidbodies().size());
	for (auto&& pr : pmx.GetRigidbodies()) {
		auto collider = LoadCollider(pr);
		Rigidbody::ConstructInfo info{};
		info.angularDamping = pr.angularDamping;
		info.bindWorld = { glm::make_vec3(pr.position), Quat{ glm::make_vec3(pr.rotation) } };
		info.collider = collider;
		info.friction = pr.friction;
		info.group = pr.group;
		switch (static_cast<PMXFile::RigidbodyType>(pr.physicsType)) {
		case PMXFile::RigidbodyType::DYNAMIC:
			info.flags |= Rigidbody::DYNAMIC_BIT;
			break; 
		case PMXFile::RigidbodyType::KINEMATIC:
			info.flags |= Rigidbody::KINEMATIC_BIT;
			break; 
		case PMXFile::RigidbodyType::PIVOTED:
			info.flags |= (Rigidbody::KINEMATIC_BIT | Rigidbody::DYNAMIC_BIT);
			break; 
		}
		info.linearDamping = pr.linearDamping;
		info.mass = pr.mass;
		info.name = pr.nameJP;
		info.noCollisionGroupMask = pr.noCollisionGroup;
		info.restitution = pr.restitution;
		auto rigidbody = pm->CreateRigidbody(info);
		bones[pr.boneIndex]->SetRigidbody(rigidbody);
		result.push_back(rigidbody);
	}
	return result;
}

static DynArray<Constraint> LoadConstraints(const PMXFile& pmx, const DynArray<Rigidbody*>& rigidbodies)
{
	auto pm = GetPhysicsManager();
	DynArray<Constraint> result;
	result.reserve(pmx.GetJoints().size());
	for (auto&& pj : pmx.GetJoints()) {
		Constraint::ConstructInfo info{};
		info.angularLimit = MakePair(glm::make_vec3(pj.angularLimit[0]),
									 glm::make_vec3(pj.angularLimit[1]));
		info.angularStiffness = glm::make_vec3(pj.angularStiffness);
		info.bindWorld = { glm::make_vec3(pj.position), Quat{glm::make_vec3(pj.rotation)} };
		info.linearLimit = MakePair(glm::make_vec3(pj.angularLimit[0]),
									glm::make_vec3(pj.angularLimit[1]));
		info.linearStiffness = glm::make_vec3(pj.linearStiffness);
		info.name = pj.nameJP;
		info.rigidbodyA = rigidbodies[pj.rigidbodyIndices[0]];
		info.rigidbodyB = rigidbodies[pj.rigidbodyIndices[1]];
		info.type = Constraint::Type::GENERIC_6DOP_SPRING;
		result.emplace_back(pm->CreateConstraint(info));
	}
	return result;
}

Model::Model(const PMXFile& pmx) :
	SceneObject{ pmx.GetInfo().nameJP }
{
	auto va = LoadVertexArray(pmx);
	m_mesh = MakeRef<Mesh>(m_name, va);
	auto textures = LoadTextures(pmx);
	LoadSubMeshes(pmx, textures, *m_mesh);
	m_armature = MakeScoped<Armature>(pmx);
	m_rigidbodies = LoadRigidbodies(pmx, m_armature->GetBones());
	m_constraints = LoadConstraints(pmx, m_rigidbodies);
}

void Model::AttachTo(Node* node)
{
	SceneObject::AttachTo(node);
	for (auto&& bone : m_armature->GetBones()) {
		if (!bone->GetParent()) {
			node->AttachObject(bone);
			node->SetWorldTransform(bone->GetBindWorld());
		} else {
			auto parentNode = bone->GetParent()->GetNode();
			auto boneNode = parentNode->AddChild(bone->GetName());
			boneNode->AttachObject(bone);
			boneNode->SetWorldTransform(bone->GetBindWorld());
		}
		auto rigidbody = bone->GetRigidbody();
		if (rigidbody) {
			bone->GetNode()->AttachObject(rigidbody);
		}
	}
}

void Model::LoadPose(const VPDFile& vpd)
{
	m_armature->LoadPose(vpd.GetPose());
}

void Model::UpdateSkinningBuffer()
{
	m_armature->UpdateSkinningBuffer();
}

}

