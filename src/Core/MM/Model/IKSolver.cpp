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

	void IKSolver::Rotate(uint32_t pivot, const glm::quat& qL, const glm::quat& qW)
	{
		m_nodes[pivot].local.rotation = m_nodes[pivot].local.rotation * qL;

		for (int32_t i = pivot; i >= 0; --i) {
			glm::vec3 offset = m_nodes[i].world.translation - m_nodes[pivot].world.translation;
			m_nodes[i].world.translation = m_nodes[pivot].world.translation + glm::rotate(qW, offset);
			m_nodes[i].world.rotation = qW * m_nodes[i].world.rotation;
		}
	}

	// http://rodolphe-vaillant.fr/entry/114/cyclic-coordonate-descent-inverse-kynematic-ccd-ik
	void IKSolver::Solve()
	{
		for (uint32_t iteration = 0; iteration < m_ik.iteration; ++iteration) {
			for (uint32_t i = 1; i < m_nodes.size(); ++i) {
				glm::vec3 o = m_nodes[i].world.translation;
				glm::vec3 E = m_nodes.front().world.translation;
				glm::vec3 T = m_armature.m_bones[m_target].animWorld.translation;
				glm::vec3 t = glm::normalize(T - o);
				glm::vec3 e = glm::normalize(E - o);
				glm::quat qW = glm::rotation(e, t);
				glm::quat qLtoW = m_nodes[i].world.rotation;
				glm::quat qL = glm::inverse(qLtoW) * qW * qLtoW;

				uint32_t linkIndex = i - 1;
				if (m_ik.link[linkIndex].doLimit) {
					glm::vec3 euler = glm::eulerAngles(qL);
					glm::vec3 low = glm::make_vec3(m_ik.link[linkIndex].limit[0]);
					glm::vec3 high = glm::make_vec3(m_ik.link[linkIndex].limit[1]);
					euler = glm::clamp(euler, low, high);
					qL = glm::quat(euler);
					qW = qLtoW * qL * glm::inverse(qLtoW);
				}

				Rotate(i, qL, qW);
			}
		}
	}

	void IKSolver::Sync()
	{
		for (uint32_t i = 0; i < m_nodes.size(); ++i) {
			m_armature.m_bones[m_nodes[i].index].animLocal = m_nodes[i].local;
		}
	}
}





