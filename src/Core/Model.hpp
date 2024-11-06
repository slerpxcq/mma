#pragma once

#include "Mesh.hpp"
#include "Bone.hpp"
#include "SceneObject.hpp"
#include "Graphics/ShaderStroageBuffer.hpp"

namespace mm
{

class PMXFile;

class Model : public SceneObject
{
public:
	Model(StringView name) : 
		SceneObject{ name } {}

	static Ref<Model> Load(Node& node, const PMXFile& pmx);

	/* BEGIN TEST CODE */
	/* END TEST CODE */

private:
	Ref<Mesh> m_mesh;
	Ref<ShaderStroageBuffer> m_skinningBuffer{};
	HashMap<String, i32> m_boneNameIndexMap;
	DynArray<Bone> m_bones{};
};

}

