#pragma once

#include "Core/Math/Transform.hpp"

#include "Node.hpp"

namespace mm
{

class PMXFile;

class ArmatureNode : public Node
{
	friend class ModelNode;
public:
	using Dictionary = std::unordered_map<std::string, int32_t>;
	enum BoneFlag : uint32_t {
		BONE_CONNECTED_BIT = (1U << 0),
		BONE_ROTATABLE_BIT = (1U << 1),
		BONE_MOVEABLE_BIT = (1U << 2),
		BONE_VISIBLE_BIT = (1U << 3),
		BONE_OPERABLE_BIT = (1U << 4),
		BONE_IK_BIT = (1U << 5),
		BONE_ASSIGN_LOCAL_BIT = (1U << 7),
		BONE_ASSIGN_ROTATION_BIT = (1U << 8),
		BONE_ASSIGN_MOVE_BIT = (1U << 9),
		BONE_FIXED_AXIS_BIT = (1U << 10),
		BONE_LOCAL_AXIS_BIT = (1U << 11),
		BONE_AFTER_PHYSICS_BIT = (1U << 12),
		BONE_EXTERNAL_PARENT_BIT = (1U << 13),
	};

	/* Inherit for selection only */
	struct Bone : public Node {
		std::string name;
		int32_t parent;
		uint32_t flags;
		uint32_t layer;
		Transform pose;
		Transform bindParent;
		Transform bindWorld;
		Transform bindWorldInv;
		Transform animLocal;
		Transform animParent;
		Transform animWorld;

		struct {
			int32_t target;
			float ratio;
		} assignment;
	};

public:
	static std::unique_ptr<ArmatureNode> Make(const PMXFile& pmx);
	ArmatureNode() { m_name = "Armature"; }
	virtual void ShowProperties() override;

	void ResetPose();
	void UpdatePose();
	void CalcSkinning();
	const Dictionary& GetDictionary() const { return m_dictionary; }
	//void SyncWithPhysics();

private:
	bool IsCurrentLayer(const Bone& bone, uint32_t layer, bool afterPhysics);
	void CalcWorldPose(uint32_t begin = 0);
	void UpdateForwardKinematics(uint32_t layer, bool afterPhysics);
	void UpdateInverseKinematics(uint32_t layer, bool afterPhysics);
	void UpdateAssignment(uint32_t layer, bool afterPhysics);

private:
	Dictionary m_dictionary;

	/* Skinning data in [q t] representation */
	std::vector<glm::mat2x4> m_skinning;

	std::vector<Bone> m_bones;

	uint32_t m_layerCount = 0;
};

}

