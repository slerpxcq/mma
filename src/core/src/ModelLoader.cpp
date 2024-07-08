#include "CorePch.hpp"
#include "ModelLoader.hpp"

#include "SkinNode.hpp"
#include "MeshNode.hpp"
#include "ArmatureNode.hpp"
#include "BoneNode.hpp"
#include "PhysicsNode.hpp"
#include "RigidbodyNode.hpp"
#include "JointNode.hpp"

#include <renderer/src/MMVertex.hpp>
#include <renderer/src/GPUResourceManager.hpp>

namespace mm
{

std::shared_ptr<ModelNode> ModelLoader::LoadFromPMX(std::shared_ptr<PMXFile> pmx)
{
	auto model =  std::make_shared<ModelNode>(pmx->GetInfo().nameJP);
	auto skin = std::make_shared<SkinNode>("Skin");
	model->AddChild(skin);

	/* Load vertices */
	std::vector<MMVertex::Layout> vertices;
	vertices.reserve(pmx->GetVertices().size());
	for (auto pmxVertex : pmx->GetVertices()) {
		MMVertex::Layout vertex = {};
		vertex.position = glm::make_vec3(pmxVertex.position);
		vertex.normal = glm::make_vec3(pmxVertex.normal);
		vertex.texCoord = glm::make_vec2(pmxVertex.uv);
		/* TODO: Load extra texCoords */
		switch (pmxVertex.blendingType) {
		case PMXFile::VERTEX_BDEF1:
			vertex.bones[0] = pmxVertex.blending.bdef1.boneIndex;
			vertex.bones[1] = -1;
			vertex.bones[2] = -1;
			vertex.bones[3] = -1;
			vertex.weights[0] = 1;
			break; 
		case PMXFile::VERTEX_BDEF2:
			vertex.bones[0] = pmxVertex.blending.bdef2.boneIndices[0];
			vertex.bones[1] = pmxVertex.blending.bdef2.boneIndices[1];
			vertex.bones[2] = -1;
			vertex.bones[3] = -1;
			vertex.weights[0] = pmxVertex.blending.bdef2.weight;
			vertex.weights[1] = 1 - vertex.weights[0];
			break;
		case PMXFile::VERTEX_BDEF4:
			vertex.bones = glm::make_vec4(pmxVertex.blending.bdef4.boneIndices);
			vertex.weights = glm::make_vec3(pmxVertex.blending.bdef4.weights);
			break;
		case PMXFile::VERTEX_SDEF:
			vertex.bones[0] = pmxVertex.blending.sdef.boneIndices[0];
			vertex.bones[1] = pmxVertex.blending.sdef.boneIndices[1];
			vertex.bones[2] = -1;
			vertex.bones[3] = 0; 	/* SDEF mark */
			vertex.weights[0] = pmxVertex.blending.sdef.weight;
			vertex.weights[1] = 1 - vertex.weights[0];
			vertex.sdef_c = glm::make_vec3(pmxVertex.blending.sdef.c);
			vertex.sdef_r0 = glm::make_vec3(pmxVertex.blending.sdef.r0);
			vertex.sdef_r1 = glm::make_vec3(pmxVertex.blending.sdef.r1);
			break;
		}
		vertices.push_back(vertex);
	}

	auto vertexBuffer = GPUResourceManager::Instance().Load<VertexBuffer>(model->GetName() + "_vertices");
	vertexBuffer->Data(vertices.size() * sizeof(vertices[0]), vertices.data());

	/* Load indices */
	auto elementBuffer = GPUResourceManager::Instance().Load<ElementBuffer>(model->GetName() + "_indices");
	elementBuffer->SetIndexSize(pmx->GetHeader().vertexIndexSize);
	const auto& faces = pmx->GetFaces();
	elementBuffer->Data(faces.size() * sizeof(faces[0]), (void*)faces.data());

	/* Load vertex array */
	auto vertexArray = GPUResourceManager::Instance().Load<VertexArray>(model->GetName() + "_vertexarray",
																		vertexBuffer,
																		elementBuffer,
																		*MMVertex::Instance().GetLayout());

	/* Load meshes */
	uint32_t offset = 0;
	for (auto pmxMesh : pmx->GetMaterials()) {
		auto mesh = std::make_shared<MeshNode>(pmxMesh.nameJP);
		skin->AddChild(mesh);
		mesh->m_indexOffset = offset;
		mesh->m_vertexArray = vertexArray;
		mesh->m_elementBuffer = elementBuffer;
		mesh->m_vertexBuffer = vertexBuffer;
		offset += pmxMesh.elementCount;
	}
	
	/* Load textures */

	/* Load armature */
	auto armature = std::make_shared<ArmatureNode>("Armature");
	model->AddChild(armature);
	for (auto pmxBone : pmx->GetBones()) {
		auto bone = std::make_shared<BoneNode>(pmxBone.nameJP);
		armature->m_bones.push_back(bone);
		if (pmxBone.parentIndex < 0) {
			armature->AddChild(bone);
			bone->SetLocalTransform(Transform(glm::make_vec3(pmxBone.position), 
											  glm::identity<glm::quat>()));
		} else {
			const auto& parent = pmx->GetBones()[pmxBone.parentIndex];
			armature->m_bones[pmxBone.parentIndex].lock()->AddChild(bone);
			glm::vec3 offset = glm::make_vec3(pmxBone.position) - glm::make_vec3(parent.position);
			bone->SetLocalTransform(Transform(offset,
											  glm::identity<glm::quat>()));
		}
	}

	/* Load physics */
	auto physics = std::make_shared<PhysicsNode>("Physics");
	model->AddChild(physics);
	/* Load rigidbodies */
	for (auto pmxRigidbody : pmx->GetRigidbodies()) {
		auto rigidbody = std::make_shared<RigidbodyNode>(pmxRigidbody.nameJP);
		physics->AddChild(rigidbody);
	}
	/* Load joints */
	for (auto pmxJoint : pmx->GetJoints()) {
		auto joint = std::make_shared<JointNode>(pmxJoint.nameJP);
		physics->AddChild(joint);
	}

	return model;
}

}

