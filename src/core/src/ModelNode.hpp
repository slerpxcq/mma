#pragma once

#include "Node.hpp"

namespace mm
{

class ModelNode : public Node
{
	friend class ModelLoader;
public:
	ModelNode(const std::string& name = "", 
			   std::shared_ptr<Node> parent = nullptr) :
		Node(name, parent) {}

private:
};

}

