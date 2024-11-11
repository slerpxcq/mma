#pragma once

#include "Mesh.hpp"
#include "Bone.hpp"
#include "SceneObject.hpp"
#include "Armature.hpp"

#include "Graphics/ShaderStroageBuffer.hpp"
#include "InverseKinematicData.hpp"

namespace mm
{

class PMXFile;
class VPDFile;

class Model : public SceneObject
{
public:
	Model(const PMXFile& pmx);

	virtual void OnUpdate(f32) override;

	/* BEGIN TEST CODE */
	void AttachTo(Node* node) override;
	void LoadPose(const VPDFile& vpd);
	void UpdateSkinningBuffer();
	/* BEGIN END CODE */

private:
	Ref<Mesh> m_mesh;
	Scoped<Armature> m_armature;
};

}

