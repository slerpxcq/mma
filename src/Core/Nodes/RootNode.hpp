#pragma once

#include "Core/Utility/Singleton.hpp"
#include "Node.hpp"

namespace mm
{

class RootNode : public Node, public Singleton<RootNode>
{
	friend class Singleton<RootNode>;
public:
	virtual void Startup() override;
	virtual void Shutdown() override;
	virtual bool IsRemoveable() override { return false; }

private:
	RootNode() : Node(nullptr) {}
};

}

