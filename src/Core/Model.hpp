#pragma once

#include "Mesh.hpp"
#include "Bone.hpp"
#include "SceneObject.hpp"
#include "Armature.hpp"

#include "Graphics/ShaderStroageBuffer.hpp"

namespace mm
{

class PMXFile;
class VPDFile;

class Model : public SceneObject
{
public:
	Model(const PMXFile& pmx);

	// virtual void OnUpdate(f32) override;
	void AttachTo(Node* node) override;

	/* BEGIN TEST CODE */
	Armature* GetArmature() const { return m_armature.get(); }
	void LoadPose(const VPDFile& vpd);
	void UpdateSkinningBuffer();
	void OnUpdate(f32) override { m_armature->Update(); }
	/* BEGIN END CODE */

private:
	Ref<Mesh> m_mesh;
	Scoped<Armature> m_armature;
};

}

