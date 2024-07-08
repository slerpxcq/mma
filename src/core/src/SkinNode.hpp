#pragma once

#include "Node.hpp"

namespace mm
{

class SkinNode : public Node
{
	friend class ModelLoader;
public:
	SkinNode(const std::string& name = "", 
			   std::shared_ptr<Node> parent = nullptr) :
		Node(name, parent) {}

private:
	uint32_t m_meshCount;
};

}

