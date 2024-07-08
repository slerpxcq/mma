#pragma once

#include "Node.hpp"

namespace mm
{

class BoneNode : public Node
{
public:
	BoneNode(const std::string& name = "", 
			   std::shared_ptr<Node> parent = nullptr) :
		Node(name, parent) {}

private:
	uint32_t m_flags;
};

}

