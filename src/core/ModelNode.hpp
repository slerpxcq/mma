#pragma once

#include "Node.hpp"
#include "Mesh.hpp"

namespace mm
{

class PMXFile;

class ModelNode : public Node
{
public:
	ModelNode(const String& name) : 
		Node{ name } {}

	static Scoped<ModelNode> LoadFromPMX(const PMXFile& pmx);

private:
	Ref<Mesh> m_mesh;
};

}

