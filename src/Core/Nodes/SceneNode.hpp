#pragma once

#include "Node.hpp"

namespace mm
{

class SceneNode : public Node
{
	friend class RootNode;
public:
	SceneNode(Node* parent) : Node(parent) {}
	virtual void ShowProperties() override;

private:

private:
};

}

