#include "mmpch.hpp"
#include "Armature.hpp"

#include "IKSolver.hpp"
#include "../Transform.hpp"
#include "Model.hpp"

#include "../Physics/ModelPhysicsData.hpp"
#include "Core/Utility/Type.hpp"

namespace mm
{
	Armature::Armature(Model &model) :
		m_model(model)
	{
		const auto& pmxBones = m_model.m_pmxFile->GetBones();
		m_layerCount = 0;

		m_bones.resize(pmxBones.size());
		m_skinningData.resize(m_bones.size());
		m_pose.resize(m_bones.size());

		for (uint32_t i = 0; i < m_bones.size(); ++i) {
			const auto& pmxBone = pmxBones[i];
			auto& bone = m_bones[i];
			bone.parent = pmxBones[i].parentIndex;

			// Bind world position
			bone.bindWorld = Transform(
				glm::make_vec3(pmxBone.position),
				glm::identity<glm::quat>());

			// Bind parent space position
			bone.bindParent = Transform(
				bone.parent >= 0 ?
					bone.bindWorld.trans - m_bones[bone.parent].bindWorld.trans :
					bone.bindWorld.trans,
				glm::identity<glm::quat>());

			bone.invBindWorld = bone.bindWorld.inverse();

			if (pmxBone.transformationLayer > m_layerCount)
				m_layerCount = pmxBone.transformationLayer;

			m_dict.insert({ pmxBone.nameJP, i });
		}

		ResetPose();
	}

	void Armature::ResetPose()
	{
		for (uint32_t i = 0; i < m_bones.size(); ++i)
			m_pose[i] = Transform::identity();
	}

	void Armature::UpdatePose()
	{
		for (uint32_t i = 0; i < m_bones.size(); ++i)
			m_bones[i].animLocal = Transform::identity();

		for (uint32_t i = 0; i <= m_layerCount; ++i) {
			UpdateForwardKinematics(i, false);
			UpdateInverseKinematics(i, false);
			UpdateAssignment(i, false);
		}
	}

	void Armature::SyncWithPhysics(const ModelPhysicsData& physicsData)
	{
		const auto& pmxRigidbodies = m_model.m_pmxFile->GetRigidbodies();

		for (uint32_t i = 0; i < pmxRigidbodies.size(); ++i) {
			const auto& pmxRigidbody = pmxRigidbodies[i];
			if (pmxRigidbody.physicsType == PMXFile::RB_DYNAMIC) {
				auto& bone = m_bones[pmxRigidbody.boneIndex];
				const btTransform& bindTransform = physicsData.m_bindTransforms[i];
				btVector3 bindOffset = bindTransform.getOrigin() - btVec3FromGLM(bone.bindWorld.trans);

				btTransform transform;
				physicsData.m_motionStates[i]->getWorldTransform(transform);

				btQuaternion rotation = transform.getRotation() * bindTransform.getRotation().inverse();
				glm::quat worldRot = btQuatToGLM(rotation);
				glm::vec3 worldTrans = btVec3ToGLM(transform.getOrigin() - bindOffset.rotate(rotation.getAxis(), rotation.getAngle()));

				bone.animWorld = Transform(worldTrans, worldRot);
				bone.animParent = m_bones[bone.parent].animWorld.inverse() * bone.animWorld;
				bone.animLocal = bone.bindParent.inverse() * bone.animParent;
			}
		}

		// After physics
		for (uint32_t i = 0; i <= m_layerCount; ++i) {
			UpdateForwardKinematics(i, true);
			UpdateInverseKinematics(i, true);
			UpdateAssignment(i, true);
		}
	}

	bool Armature::IsCurrentLayer(uint32_t index, uint32_t layer, bool afterPhys)
	{
		const auto& pmxBones = m_model.m_pmxFile->GetBones();

		return pmxBones[index].transformationLayer == layer &&
			(bool)(pmxBones[index].flags & PMXFile::BONE_AFTER_PHYSICS_BIT) == afterPhys;
	}

	void Armature::UpdateForwardKinematics(uint32_t layer, bool afterPhys)
	{
		for (uint32_t i = 0; i < m_bones.size(); ++i)
			if (IsCurrentLayer(i, layer, afterPhys))
				m_bones[i].animLocal = m_pose[i] * m_bones[i].animLocal;

		CalcWorldPose();
	}

	void Armature::UpdateInverseKinematics(uint32_t layer, bool afterPhys)
	{
		const auto& pmxBones = m_model.m_pmxFile->GetBones();

		for (uint32_t i = 0; i < m_bones.size(); ++i) {
			if (IsCurrentLayer(i, layer, afterPhys) &&
				pmxBones[i].flags & PMXFile::BONE_IK_BIT) {
				IKSolver solver(*this, pmxBones[i].ik, i);
				solver.Solve();
				solver.Sync(); 
				CalcWorldPose(pmxBones[i].ik.link.back().boneIndex);
			}
		}
	}

	void Armature::UpdateAssignment(uint32_t layer, bool afterPhys)
	{
		const auto& pmxBones = m_model.m_pmxFile->GetBones();

		for (uint32_t i = 0; i < m_bones.size(); ++i) {
			if (IsCurrentLayer(i, layer, afterPhys)) {
				const auto& assignment = pmxBones[i].assignment;
				bool doAssignment = false;
				Transform xform = Transform::identity();

				if (pmxBones[i].flags & PMXFile::BONE_ASSIGN_MOVE_BIT) {
					xform.trans = assignment.ratio * m_bones[assignment.targetIndex].animLocal.trans;
					doAssignment = true;
				}

				if (pmxBones[i].flags & PMXFile::BONE_ASSIGN_ROTATION_BIT) {
					xform.rot = glm::slerp(
						glm::identity<glm::quat>(),
						m_bones[assignment.targetIndex].animLocal.rot,
						assignment.ratio);
					doAssignment = true;
				}

				if (doAssignment) {
					m_bones[i].animLocal = xform * m_bones[i].animLocal;
				}
			}
		}

		CalcWorldPose();
	}

	void Armature::CalcWorldPose(uint32_t begin)
	{
		for (uint32_t i = begin; i < m_bones.size(); ++i) {
			m_bones[i].animParent = m_bones[i].bindParent * m_bones[i].animLocal;
			m_bones[i].animWorld = m_bones[i].parent >= 0 ?
				m_bones[m_bones[i].parent].animWorld * m_bones[i].animParent :
				m_bones[i].animParent;
		}
	}

	void Armature::CalcSkinning()
	{
		for (uint32_t i = 0; i < m_bones.size(); ++i) {
			Transform skinning = m_bones[i].animWorld * m_bones[i].invBindWorld;
			m_skinningData[i][0] = glm::make_vec4(glm::value_ptr(skinning.rot));
			m_skinningData[i][1] = glm::vec4(skinning.trans, 1);
		}

		m_model.m_skinningBuffer->SetSubData(
			0, 
			m_skinningData.size() * sizeof(glm::mat2x4), 
			m_skinningData.data());
	}
}



