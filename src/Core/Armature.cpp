#include "CorePch.hpp"
#include "Armature.hpp"

#include "SceneManager.hpp"
#include "File/PMXFile.hpp"
#include "Bone.hpp"

namespace mm
{

Armature::Armature(const PMXFile& pmx)
{
	auto sm = GetSceneManager();
	m_bones.reserve(pmx.GetBones().size());
	i32 index{0};
	for (const auto& pb : pmx.GetBones()) {
		/* Load IK data*/
		if (pb.flags & PMXFile::IK_BIT) {
		}
		/* Load position  */
		Transform bindWorld = { glm::make_vec3(pb.position), glm::identity<Quat>() };
		Transform bindLocal = { pb.parentIndex < 0 ?
			glm::make_vec3(pb.position) :
			glm::make_vec3(pb.position) - glm::make_vec3(pmx.GetBones()[pb.parentIndex].position),
			glm::identity<Quat>() };
		Bone::BoneConstructInfo info{};
		info.name = pb.nameJP;
		info.flags = pb.flags;
		info.parentIndex = pb.parentIndex;
		info.transformLayer = pb.transformationLayer;
		info.bindLocal = bindLocal;
		info.bindWorld = bindWorld;
		auto bone = sm->CreateObject<Bone>(info);
		m_boneNameIndexMap.insert({ pb.nameJP, index });
		m_maxTransformLayer = std::max(m_maxTransformLayer, static_cast<u32>(pb.transformationLayer));
		m_bones.push_back(bone);
		++index;
	}
	m_skinningBuffer.SetStorage(nullptr,
								 m_bones.size() * sizeof(Mat4),
								 Graphics::BufferFlags::MAP_WRITE_BIT);
	m_skinningBuffer.SetBindBase(0);
}

}
