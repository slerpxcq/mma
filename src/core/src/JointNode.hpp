#pragma once

#include "Node.hpp"
#include "RigidbodyNode.hpp"

namespace mm
{

class JointNode : public Node
{
public:
	JointNode(const std::string& name = "", 
			   std::shared_ptr<Node> parent = nullptr) :
		Node(name, parent) {}

private:
	std::weak_ptr<RigidbodyNode> m_rigidbodies[2];
};

}

