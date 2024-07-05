#pragma once

#include "Node.hpp"

namespace mm
{

class SceneNode : public Node
{
public:
	static std::shared_ptr<SceneNode> CreateDefault();

private:
};

}

