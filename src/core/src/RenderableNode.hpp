#pragma once

#include "Node.hpp"

namespace mm
{

class RenderableNode : public Node
{
public:
	RenderableNode(const std::string& name = "", 
			   std::shared_ptr<Node> parent = nullptr) :
		Node(name, parent) {}

	virtual void Render() {}

private:
};

}

