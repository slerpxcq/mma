#pragma once

#include "Node.hpp"
#include "Mesh.hpp"
#include "Graphics/Texture.hpp"

namespace mm
{

class PMXFile;

class ModelNode : public Node
{
public:
	ModelNode(const String& name) : 
		Node{ name } {}

	static Scoped<ModelNode> LoadFromPMX(const PMXFile& pmx);
	const auto& GetMeshes() const { return m_meshes; }

private:
	DynArray<Ref<Mesh>> m_meshes{};
	DynArray<Ref<Texture>> m_textures{};
};

}

