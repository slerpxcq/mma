#pragma once

#include "Node.hpp"
#include "Mesh.hpp"

namespace mm
{

class ModelNode : public Node
{
	// TODO: is this good?
	friend class PMXLoader;
public:
	ModelNode(const String& name) : 
		Node{ name } {}

private:
	Ref<Mesh> m_mesh;
};

}

