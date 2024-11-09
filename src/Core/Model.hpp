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

	/* BEGIN TEST CODE */
	static Ref<Model> Load(const PMXFile& pmx);
	void AttachTo(Node& node) override;
	void LoadPose(const VPDFile& vpd);
	void UpdateSkinningBuffer();
	/* BEGIN END CODE */

private:
	Ref<Mesh> m_mesh;
	Ref<ShaderStroageBuffer> m_skinningBuffer{};
	HashMap<String, i32> m_boneNameIndexMap;
	DynArray<Ref<Bone>> m_bones{};
};

}

