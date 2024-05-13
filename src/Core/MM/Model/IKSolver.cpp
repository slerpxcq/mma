#include "mmpch.hpp"
#include "IKSolver.hpp"

#include "Armature.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

namespace mm
{
	IKSolver::IKSolver(Armature& armature, const PMXFile::Bone::IK& ik, int32_t target) :
		m_armature(armature),
		m_ik(ik),
		m_target(target)
	{
		m_nodes.push_back({
			m_ik.targetIndex,
			m_armature.m_bones[m_ik.targetIndex].animWorld,
			m_armature.m_bones[m_ik.targetIndex].animLocal });

		for (uint32_t i = 0; i < m_ik.link.size(); ++i) {
			int32_t index = m_ik.link[i].boneIndex;
			m_nodes.push_back({
				index,
				m_armature.m_bones[index].animWorld,
				m_armature.m_bones[index].animLocal });
		}
	}

	void IKSolver::Rotate(uint32_t pivot, const glm::quat& q_local, const glm::quat& q_world)
	{
		m_nodes[pivot].local.rotation = m_nodes[pivot].local.rotation * q_local;

		for (int32_t i = pivot; i >= 0; --i) {
			glm::vec3 offset = m_nodes[i].world.translation - m_nodes[pivot].world.translation;
			m_nodes[i].world.translation = m_nodes[pivot].world.translation + glm::rotate(q_world, offset);
			m_nodes[i].world.rotation = q_world * m_nodes[i].world.rotation;
		}
	}

	// http://rodolphe-vaillant.fr/entry/114/cyclic-coordonate-descent-inverse-kynematic-ccd-ik
	// https://stackoverflow.com/questions/21373012/best-inverse-kinematics-algorithm-with-constraints-on-joint-angles
	// http://kzntov.seesaa.net/article/455959577.html?seesaa_related=category
	void IKSolver::Solve()
	{
		for (uint32_t iteration = 0; iteration < m_ik.iteration; ++iteration) {
			for (uint32_t i = 1; i < m_nodes.size(); ++i) {
				glm::vec3 o = m_nodes[i].world.translation;
				glm::vec3 E = m_nodes[0].world.translation;
				glm::vec3 T = m_armature.m_bones[m_target].animWorld.translation;
				glm::vec3 t = glm::normalize(T - o);
				glm::vec3 e = glm::normalize(E - o);
				glm::quat q_world = glm::rotation(e, t);
				glm::quat q_local_to_world = m_nodes[i].world.rotation;
				glm::quat q_local = glm::inverse(q_local_to_world) * q_world * q_local_to_world;

				uint32_t linkIndex = i - 1;
				if (m_ik.link[linkIndex].doLimit) {
					glm::vec3 low = glm::make_vec3(m_ik.link[linkIndex].limit[0]);
					glm::vec3 high = glm::make_vec3(m_ik.link[linkIndex].limit[1]);
					glm::vec3 euler_ideal = glm::eulerAngles(m_nodes[i].local.rotation * q_local);
					glm::vec3 euler_clamped = glm::clamp(euler_ideal, low, high);
					glm::quat q_clamped = glm::quat(euler_clamped);

					q_local = glm::inverse(m_nodes[i].local.rotation) * q_clamped;
					q_world = q_local_to_world * q_local * glm::inverse(q_local_to_world);
				}

				Rotate(i, q_local, q_world);
			}
		}
	}

	void IKSolver::Sync()
	{
		for (uint32_t i = 0; i < m_nodes.size(); ++i) 
			m_armature.m_bones[m_nodes[i].index].animLocal = m_nodes[i].local;
	}
}





