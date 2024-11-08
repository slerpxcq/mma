#pragma once

#include "Mesh.hpp"
#include "Bone.hpp"
#include "SceneObject.hpp"
#include "Graphics/ShaderStroageBuffer.hpp"

namespace mm
{

class PMXFile;
class VPDFile;

class Model : public SceneObject
{
public:
	Model(StringView name) : 
		SceneObject{ name } {}

	virtual void OnUpdate(f32) override;
	static Ref<Model> Load(Node& node, const PMXFile& pmx);
	void LoadPose(const VPDFile& vpd);
	void UpdateSkinningBuffer();

private:
	Ref<Mesh> m_mesh;
	Ref<ShaderStroageBuffer> m_skinningBuffer{};
	HashMap<String, i32> m_boneNameIndexMap;
	DynArray<Ref<Bone>> m_bones{};
};

}

