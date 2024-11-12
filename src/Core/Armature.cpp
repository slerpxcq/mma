#include "CorePch.hpp"
#include "Armature.hpp"

#include "SceneManager.hpp"
#include "File/PMXFile.hpp"
#include "Bone.hpp"

#include "InverseKinematics/InverseKinematicsInfo.hpp"
#include "InverseKinematics/InverseKinematicsSolver.hpp"

namespace mm
{

Armature::Armature(const PMXFile& pmx)
{
	LoadBonesPass1(pmx);
	LoadBonesPass2(pmx);

	m_skinningBuffer.SetBindBase(0);
	m_skinningBuffer.SetStorage(nullptr,
								m_bones.size() * sizeof(Mat4),
								Graphics::BufferFlags::MAP_WRITE_BIT);
}

static bool IsCurrentLayer(Bone* bone, u32 layer, bool afterPhysics)
{
	return ((bone->GetFlags() & Bone::Flags::AFTER_PHYSICS_BIT) == afterPhysics &&
			bone->GetTransformLayer() == layer);
}

void Armature::Update()
{
	for (u32 layer = 0; layer < m_maxTransformLayer; ++layer) {
		UpdateInverseKinematics(layer, false);
		UpdateAssignment(layer, false);
	}
	UpdateSkinningBuffer();
}

void Armature::LoadPose(const Pose& pose)
{
	for (auto&& [name, localTransform]: pose) {
		auto it = m_boneNameIndexMap.find(name);
		if (it != m_boneNameIndexMap.end()) {
			auto& bone = m_bones[it->second];
			bone->SetAnimLocal(localTransform);
		}
	}
	Update();
}

void Armature::UpdateSkinningBuffer()
{
	Mat4* ptr = reinterpret_cast<Mat4*>(m_skinningBuffer.Map(Graphics::BufferAccess::WRITE));
	for (auto& bone : m_bones) {
		*ptr++ = bone->GetNode()->GetWorldMatrix() * bone->GetBindWorldInverse().ToMat4();
	}
	m_skinningBuffer.Unmap();
}

void Armature::LoadBonesPass1(const PMXFile& pmx)
{
	auto sm = GetSceneManager();
	m_bones.reserve(pmx.GetBones().size());
	i32 index{};
	for (const auto& pb : pmx.GetBones()) {
		Bone::ConstructInfo info{};
		Transform bindWorld = { glm::make_vec3(pb.position), glm::identity<Quat>() };
		Transform bindLocal = { pb.parentIndex < 0 ?
			glm::make_vec3(pb.position) :
			glm::make_vec3(pb.position) - glm::make_vec3(pmx.GetBones()[pb.parentIndex].position),
			glm::identity<Quat>() };
		info.name = pb.nameJP;
		info.index = index;
		info.flags = pb.flags;
		info.transformLayer = pb.transformationLayer;
		info.bindLocal = bindLocal;
		info.bindWorld = bindWorld;
		auto bone = sm->CreateObject<Bone>(info);
		m_boneNameIndexMap.insert({ pb.nameJP, index });
		m_maxTransformLayer = std::max(m_maxTransformLayer, static_cast<u32>(pb.transformationLayer));
		m_bones.push_back(bone);
		++index;
	}
}

void Armature::LoadBonesPass2(const PMXFile& pmx)
{
	i32 index{};
	for (auto& pb : pmx.GetBones()) {
		auto& bone = m_bones[index];
		if ((pb.flags & PMXFile::BoneFlag::CONNECTED_BIT) &&
			(pb.connetcionEnd.boneIndex >= 0)) {
			bone->SetTipBone(m_bones[pb.connetcionEnd.boneIndex]);
		} else {
			bone->SetTipOffset(glm::make_vec3(pb.connetcionEnd.position));
		}
		if (pb.flags & PMXFile::BoneFlag::IK_BIT) {
			InverseKinematicsInfo ikInfo{};
			ikInfo.iteration = pb.ik.iteration;
			ikInfo.unitAngle = pb.ik.unitAngle;
			ikInfo.link.reserve(pb.ik.link.size() + 1);
			/* End effector */
			InverseKinematicsInfo::Node node{};
			node.bone = m_bones[pb.ik.targetIndex];
			node.hasLimit = false;
			ikInfo.link.push_back(node);
			for (auto& pn : pb.ik.link) {
				InverseKinematicsInfo::Node node{};
				node.bone = m_bones[pn.boneIndex];
				node.hasLimit = pn.doLimit;
				node.lowerLimit = glm::make_vec3(pn.limit[0]);
				node.upperLimit = glm::make_vec3(pn.limit[1]);
				ikInfo.link.push_back(node);
			}
			bone->SetInverseKinematicsInfo(ikInfo);
		}
		if (pb.flags & PMXFile::BoneFlag::ASSIGN_ROTATION_BIT || 
			pb.flags & PMXFile::BoneFlag::ASSIGN_MOVE_BIT) {
			Bone::AssignmentInfo info{};
			if (pb.flags & PMXFile::BoneFlag::ASSIGN_ROTATION_BIT)
				info.type |= Bone::AssignmentInfo::ROTATION;
			if (pb.flags & PMXFile::BoneFlag::ASSIGN_MOVE_BIT)
				info.type |= Bone::AssignmentInfo::TRANSLATION;
			info.ratio = pb.assignment.ratio;
			info.target = m_bones[pb.assignment.targetIndex];
			bone->SetAssignmentInfo(info);
		}
		if (pb.parentIndex >= 0) {
			bone->SetParent(m_bones[pb.parentIndex]);
		}
		++index;
	}
}

void Armature::UpdateInverseKinematics(u32 layer, bool afterPhysics)
{
	for (auto& bone : m_bones) {
		if (IsCurrentLayer(bone, layer, afterPhysics)) {
			auto info = bone->GetInverseKinematicsInfo();
			if (info) {
				auto solver = GetInverseKinematicsSolver();
				solver->Solve(*info, bone);
			}
		}
	}
}

void Armature::UpdateAssignment(u32 layer, bool afterPhysics)
{
	for (auto& bone : m_bones) {
		if (IsCurrentLayer(bone, layer, afterPhysics)) {
			auto info = bone->GetAssignmentInfo();
			if (info) {
				Transform transform;
				if (info->type & Bone::AssignmentInfo::TRANSLATION) {
					transform.translation = info->ratio * info->target->GetAnimLocal().translation;
				}
				if (info->type & Bone::AssignmentInfo::ROTATION) {
					transform.rotation = glm::slerp(glm::identity<Quat>(),
													info->target->GetAnimLocal().rotation,
													info->ratio);
				}
				bone->SetAnimLocal(transform * bone->GetAnimLocal());
			}
		}
	}
}

}
