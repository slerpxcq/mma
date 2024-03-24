#include "mmpch.hpp"
#include "Armature.hpp"
#include "IKSolver.hpp"
//#include "PhysicsSolver.hpp"
#include "Transform.hpp"
#include "Model.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace mm
{
	Armature::Armature(Model &model) :
		m_model(model)
	{
		const auto& pmxBones = m_model.m_pmxFile->GetBones();
		m_layerCount = 0;

		m_bones.resize(pmxBones.size());
		m_skinningMatrices.resize(m_bones.size());
		m_pose.resize(m_bones.size());

		m_skinningSSBO = std::make_unique<GLBuffer>(GL_SHADER_STORAGE_BUFFER);
		m_skinningSSBO->Data(m_skinningMatrices.size() * sizeof(glm::mat4), nullptr);
		m_skinningSSBO->Base(MM_SKINNING_BASE);

		for (uint32_t i = 0; i < m_bones.size(); ++i) {
			const auto& pmxBone = pmxBones[i];
			auto& bone = m_bones[i];
			bone.parent = pmxBones[i].parentIndex;
			bone.bindWorld.rot = glm::identity<glm::quat>();
			bone.bindWorld.trans = glm::make_vec3(pmxBone.position);
			bone.bindParent.rot = glm::identity<glm::quat>();
			bone.bindParent.trans = bone.parent >= 0 ?
				bone.bindWorld.trans - m_bones[bone.parent].bindWorld.trans :
				bone.bindWorld.trans;
			bone.invBindWorld = bone.bindWorld.inverse();

			if (pmxBone.transformationLayer > m_layerCount)
				m_layerCount = pmxBone.transformationLayer;
		}

		ResetPose();
	}

	void Armature::ResetPose()
	{
		for (uint32_t i = 0; i < m_bones.size(); ++i)
			m_pose[i] = Transform::identity();
	}

	void Armature::LoadPose(const std::vector<Transform>& locXforms)
	{
		for (uint32_t i = 0; i < m_bones.size(); ++i)
			m_pose[i] = locXforms[i];
	}

	void Armature::UpdatePose()
	{
		for (uint32_t i = 0; i < m_bones.size(); ++i)
			m_bones[i].animLocal = Transform::identity();

		for (uint32_t i = 0; i <= m_layerCount; ++i) {
			UpdateFK(i, false);
			UpdateIK(i, false);
			UpdateAssignment(i, false);
		}
	}

	//void Armature::SyncWithPhysics(const PhysicsSolver& solver)
	//{
	//	const auto& pmxRigidbodies = m_model.m_pmxFile->GetRigidbodies();

	//	for (uint32_t i = 0; i < pmxRigidbodies.size(); ++i) {
	//		const auto& pmxRigidbody = pmxRigidbodies[i];
	//		if (pmxRigidbody.physicsType == PMXFile::RB_DYNAMIC) {
	//			auto& bone = m_bones[pmxRigidbody.boneIndex];
	//			const btTransform& bindTransform = solver.GetBindTransforms()[i];
	//			btVector3 bindOffset = bindTransform.getOrigin() - btVec3FromGLM(bone.bindWorld.trans);

	//			btTransform transform;
	//			solver.GetMotionStates()[i]->getWorldTransform(transform);

	//			btQuaternion rotation = transform.getRotation() * bindTransform.getRotation().inverse();
	//			glm::quat worldRot = btQuatToGLM(rotation);
	//			glm::vec3 worldTrans = btVec3ToGLM(transform.getOrigin() - bindOffset.rotate(rotation.getAxis(), rotation.getAngle()));

	//			bone.animWorld = Transform(worldTrans, worldRot);
	//			bone.animParent = m_bones[bone.parent].animWorld.inverse() * bone.animWorld;
	//			bone.animLocal = bone.bindParent.inverse() * bone.animParent;
	//		}
	//	}

	//	// After physics
	//	for (uint32_t i = 0; i <= m_layerCount; ++i) {
	//		UpdateFK(i, true);
	//		UpdateIK(i, true);
	//		UpdateAssignment(i, true);
	//	}
	//}

	bool Armature::IsCurrentLayer(uint32_t index, uint32_t layer, bool afterPhys)
	{
		const auto& pmxBones = m_model.m_pmxFile->GetBones();

		return pmxBones[index].transformationLayer == layer &&
			(bool)(pmxBones[index].flags & PMXFile::BONE_AFTER_PHYSICS) == afterPhys;
	}

	void Armature::UpdateFK(uint32_t layer, bool afterPhys)
	{
		for (uint32_t i = 0; i < m_bones.size(); ++i)
			if (IsCurrentLayer(i, layer, afterPhys))
				m_bones[i].animLocal = m_pose[i] * m_bones[i].animLocal;

		CalcWorldPose();
	}

	void Armature::UpdateIK(uint32_t layer, bool afterPhys)
	{
		const auto& pmxBones = m_model.m_pmxFile->GetBones();

		for (uint32_t i = 0; i < m_bones.size(); ++i) {
			if (IsCurrentLayer(i, layer, afterPhys) &&
				pmxBones[i].flags & PMXFile::BONE_IK) {
				IKSolver solver(*this, pmxBones[i].ik, i);
				solver.Solve();
				solver.Sync();
				CalcWorldPose();
			}
		}
	}

	void Armature::UpdateAssignment(uint32_t layer, bool afterPhys)
	{
		const auto& pmxBones = m_model.m_pmxFile->GetBones();

		for (uint32_t i = 0; i < m_bones.size(); ++i) {
			if (IsCurrentLayer(i, layer, afterPhys)) {
				const auto& assn = pmxBones[i].assignment;
				bool doAssn = false;
				Transform xform = Transform::identity();

				if (pmxBones[i].flags & PMXFile::BONE_ASSIGN_MOVE) {
					xform.trans = assn.ratio * m_bones[assn.targetIndex].animLocal.trans;
					doAssn = true;
				}

				if (pmxBones[i].flags & PMXFile::BONE_ASSIGN_ROTATION) {
					xform.rot = glm::slerp(
						glm::identity<glm::quat>(),
						m_bones[assn.targetIndex].animLocal.rot,
						assn.ratio);
					doAssn = true;
				}

				if (doAssn) {
					m_bones[i].animLocal = xform * m_bones[i].animLocal;
				}
			}
		}

		CalcWorldPose();
	}

	void Armature::CalcWorldPose()
	{
		// Pj'->p(j) = Pp(j)->w * Pj'->j
		// Pj'->W = Pp(j)->w * Pj'->p(j)

		for (uint32_t i = 0; i < m_bones.size(); ++i) {
			m_bones[i].animParent = m_bones[i].bindParent * m_bones[i].animLocal;
			m_bones[i].animWorld = m_bones[i].parent >= 0 ?
				m_bones[m_bones[i].parent].animWorld * m_bones[i].animParent :
				m_bones[i].animParent;
		}
	}

	void Armature::CalcSkinning()
	{
		for (uint32_t i = 0; i < m_bones.size(); ++i)
			m_skinningMatrices[i] = Transform::toMat4(m_bones[i].animWorld * m_bones[i].invBindWorld);

		m_skinningSSBO->SubData(
			0, 
			m_skinningMatrices.size() * sizeof(glm::mat4), 
			m_skinningMatrices.data());
	}
}



