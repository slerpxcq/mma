#pragma once

#include "Graphics/ShaderStroageBuffer.hpp"
#include "InverseKinematicData.hpp"

namespace mm
{
class Model;
class PMXFile;
class Bone;
class Armature
{
public:
	Armature() = default;
	Armature(const PMXFile& pmx);
	const auto& GetBones() const { return m_bones; }
	const auto& GetBoneNameIndexMap() const { return m_boneNameIndexMap; }
	const auto& GetSkinningBuffer() const { return m_skinningBuffer; }

private:
	u32 m_maxTransformLayer{};
	DynArray<Bone*> m_bones;
	HashMap<String, i32> m_boneNameIndexMap;
	HashMap<i32, InverseKinematicData> m_ikData;

	ShaderStroageBuffer m_skinningBuffer{};
};

}

