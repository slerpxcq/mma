#pragma once

#include "Mesh.hpp"
#include "SceneObject.hpp"
#include "Graphics/Texture.hpp"

namespace mm
{

struct PMXFile;

class Model : public SceneObject
{
public:
	Model(StringView name) : 
		SceneObject{ name } {}

	static Ref<Model> LoadFromPMX(const PMXFile& pmx);

private:
	Ref<Mesh> m_mesh;
};

}

