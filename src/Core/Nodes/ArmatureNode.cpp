#include "mmpch.hpp"
#include "ArmatureNode.hpp"

#include "ModelNode.hpp"

#include "Core/Manager/PMXFile.hpp"

namespace mm
{

std::unique_ptr<ArmatureNode> ArmatureNode::Make(const PMXFile& pmx)
{
	auto armature = std::make_unique<ArmatureNode>();
	const auto& pmx_bones = pmx.GetBones();
	auto& bones = armature->m_bones;

	armature->m_skinning.resize(pmx_bones.size());
	bones.resize(pmx_bones.size());

	for (uint32_t boneIndex = 0; boneIndex < pmx_bones.size(); ++boneIndex) {
		const auto& pmx_bone = pmx_bones[boneIndex];
		auto& bone = armature->m_bones[boneIndex];

		bone.name = pmx_bone.nameJP;
		bone.parent = pmx_bone.parentIndex;
		bone.bindWorld = Transform(glm::make_vec3(pmx_bone.position),
								   glm::identity<glm::quat>());
		bone.bindWorldInv = bone.bindWorld.inverse();

		glm::vec3 bindParent = bone.parent >= 0 ?
			bone.bindWorld.translation - bones[bone.parent].bindWorld.translation :
			bone.bindWorld.translation;
		bone.bindParent = Transform(bindParent, glm::identity<glm::quat>());

		bone.flags = pmx_bone.flags;
		bone.layer = pmx_bone.layer;

		bone.assignment.target = pmx_bone.assignment.targetIndex;
		bone.assignment.ratio = pmx_bone.assignment.ratio;

		if (pmx_bone.layer > armature->m_layerCount) {
			armature->m_layerCount = pmx_bone.layer;
		}

		armature->m_dictionary.insert({ pmx_bone.nameJP, boneIndex });
	}

	return armature;
}

void ArmatureNode::ShowProperties()
{
	for (const auto& bone : m_bones) {
		ImGui::Text(bone.name.c_str());
	}
}

void ArmatureNode::ResetPose()
{
	for (auto& bone : m_bones) {
		bone.pose = Transform::identity();
	}
}

void ArmatureNode::UpdatePose()
{
	for (auto& bone : m_bones) {
		bone.animLocal = Transform::identity();
	}

	for (uint32_t layer = 0; layer <= m_layerCount; ++layer) {
		UpdateForwardKinematics(layer, false);
		UpdateInverseKinematics(layer, false);
		UpdateAssignment(layer, false); 
	}
}

void ArmatureNode::CalcSkinning()
{
	for (uint32_t boneIndex = 0; boneIndex < m_bones.size(); ++boneIndex) {
		Transform skinning = m_bones[boneIndex].animWorld * m_bones[boneIndex].bindWorldInv;
		m_skinning[boneIndex][0] = glm::make_vec4(glm::value_ptr(skinning.rotation));
		m_skinning[boneIndex][1] = glm::vec4(skinning.translation, 1);
	}
}

bool ArmatureNode::IsCurrentLayer(const Bone& bone, uint32_t layer, bool afterPhysics)
{
	return (bone.layer == layer) && ((bool)(bone.flags & BONE_AFTER_PHYSICS_BIT) == afterPhysics);
}

void ArmatureNode::CalcWorldPose(uint32_t begin)
{
	for (uint32_t boneIndex = begin; boneIndex < m_bones.size(); ++boneIndex) {
		m_bones[boneIndex].animParent = m_bones[boneIndex].bindParent * m_bones[boneIndex].animLocal;
		m_bones[boneIndex].animWorld = m_bones[boneIndex].parent >= 0 ?
			m_bones[m_bones[boneIndex].parent].animWorld * m_bones[boneIndex].animParent :
			m_bones[boneIndex].animParent;
	}
}

void ArmatureNode::UpdateForwardKinematics(uint32_t layer, bool afterPhysics)
{
	for (uint32_t boneIndex = 0; boneIndex < m_bones.size(); ++boneIndex) {
		auto& bone = m_bones[boneIndex];
		if (IsCurrentLayer(bone, layer, afterPhysics)) {
			bone.animLocal = bone.pose * bone.animLocal;
		}
	}

	CalcWorldPose();
}

void ArmatureNode::UpdateInverseKinematics(uint32_t layer, bool afterPhysics)
{
	for (uint32_t boneIndex = 0; boneIndex < m_bones.size(); ++boneIndex) {
		auto& bone = m_bones[boneIndex];
		if ((bool)(bone.flags & BONE_IK_BIT) &&
			IsCurrentLayer(bone, layer, afterPhysics)) {
			IKSolver solver(*this, pmxBones[i].ik, i);
			solver.Solve();
			solver.Sync(); 
			CalcWorldPose(pmxBones[i].ik.link.back().boneIndex);
		}
	}
}

void ArmatureNode::UpdateAssignment(uint32_t layer, bool afterPhysics)
{
	for (uint32_t boneIndex = 0; boneIndex < m_bones.size(); ++boneIndex) {
		auto& bone = m_bones[boneIndex];
		if (IsCurrentLayer(bone, layer, afterPhysics)) {
			bool doAssignment = false;
			Transform transform;

			if (bone.flags & BONE_ASSIGN_MOVE_BIT) {
				transform.translation = bone.assignment.ratio * m_bones[bone.assignment.target].animLocal.translation;
				doAssignment = true;
			}

			if (bone.flags & BONE_ASSIGN_ROTATION_BIT) {
				transform.rotation = glm::slerp(glm::identity<glm::quat>(),
												m_bones[bone.assignment.target].animLocal.rotation,
												bone.assignment.ratio);
				doAssignment = true;
			}

			if (doAssignment) {
				bone.animLocal = transform * bone.animLocal;
			}
		}
	}

	CalcWorldPose();
}

}

