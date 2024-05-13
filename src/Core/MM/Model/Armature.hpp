#pragma once

#include "../Files/PMXFile.hpp"
#include "Core/MM/Math/Transform.hpp" 

#include "Core/GL/GLBuffer.hpp"

namespace mm
{
	class PhysicsSolver;
	class Model;
	class ModelPhysicsData;

	class Armature
	{
		friend class IKSolver;
	public:
		using Dict = std::unordered_map<std::string, int32_t>;

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
		void ResetPose();
		void UpdatePose();
		void CalcSkinning();
		const Dict& GetDict() const { return m_dict; }
		void SyncWithPhysics(const ModelPhysicsData& physicsData);

		std::vector<Transform>& GetPose() { return m_pose; }
		const std::vector<Bone>& GetBones() const { return m_bones; }

	private:
		void CalcWorldPose(uint32_t begin = 0);
		void UpdateForwardKinematics(uint32_t layer, bool afterPhys);
		void UpdateInverseKinematics(uint32_t layer, bool afterPhys);
		void UpdateAssignment(uint32_t layer, bool afterPhys);
		bool IsCurrentLayer(uint32_t index, uint32_t layer, bool afterPhys);

	private:
		Model& m_model;

		/* nameJP->index */
		Dict m_dict;
		uint32_t m_layerCount;
		std::vector<Bone> m_bones;
		std::vector<Transform> m_pose;

		std::vector<glm::mat2x4> m_skinningData;
	}; 
}

