#pragma once

#include "Bone.hpp"
#include "Pose.hpp"

#include "Graphics/ShaderStroageBuffer.hpp"

namespace mm
{
class Model;
class PMXFile;

// TODO: Builder pattern
class Armature
{
public:
	Armature() = default;
	Armature(const PMXFile& pmx);
	const auto& GetBones() const { return m_bones; }
	const auto& GetBoneNameIndexMap() const { return m_boneNameIndexMap; }
	const auto& GetSkinningBuffer() const { return m_skinningBuffer; }
	void Update();
	void LoadPose(const Pose& pose);
	void UpdateSkinningBuffer();

private:
	void LoadBonesPass1(const PMXFile& pmx, DynArray<Bone::Builder>& builders);
	void LoadBonesPass2(const PMXFile& pmx, DynArray<Bone::Builder>& builders);
	void ClearAnimLocal();
	void UpdateForwardKinematics(u32 layer, bool afterPhysics);
	void UpdateInverseKinematics(u32 layer, bool afterPhysics);
	void UpdateAssignment(u32 layer, bool afterPhysics);
	void UpdatePhysics();

private:
	u32 m_maxTransformLayer{};
	DynArray<Bone*> m_bones;
	HashMap<String, i32> m_boneNameIndexMap;

	ShaderStroageBuffer m_skinningBuffer{};
};

}

