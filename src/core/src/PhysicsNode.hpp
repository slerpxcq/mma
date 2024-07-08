#pragma once

#include "Node.hpp"

namespace mm
{

class PhysicsNode : public Node
{
public:
	PhysicsNode(const std::string& name = "", 
			   std::shared_ptr<Node> parent = nullptr) :
		Node(name, parent) {}

private:
};

}

