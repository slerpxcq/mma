#pragma once

#include "PMXFile.hpp"
#include "Transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace mm
{
	class PhysicsSolver;
	class Model;

	class Armature
	{
		friend class IKSolver;
	public:
		struct Bone {
			int32_t parent;
			Transform bindParent;
			Transform bindWorld;
			Transform invBindWorld;
			Transform animLocal;
			Transform animParent;
			Transform animWorld;
		};

	public:
		explicit Armature(Model &model);
		void LoadPose(const std::vector<Transform>& locXforms);
		void ResetPose();
		void UpdatePose();
		void CalcSkinning();
		//void SyncWithPhysics(const PhysicsSolver& ps);

		const std::vector<glm::mat4>& GetSkinning() const {
			return m_skinningMatrices;
		};

		const std::vector<Bone>& GetBones() const {
			return m_bones;
		}

		std::vector<Transform>& GetPose() {
			return m_pose;
		}

	private:
		void CalcWorldPose();
		void UpdateFK(uint32_t layer, bool afterPhys);
		void UpdateIK(uint32_t layer, bool afterPhys);
		void UpdateAssignment(uint32_t layer, bool afterPhys);
		bool IsCurrentLayer(uint32_t index, uint32_t layer, bool afterPhys);

	private:
		Model& m_model;
		uint32_t m_layerCount;
		std::vector<Bone> m_bones;
		std::vector<Transform> m_pose;
		std::vector<glm::mat4> m_skinningMatrices;
		std::unique_ptr<GLBuffer> m_skinningSSBO;
	}; 
}

