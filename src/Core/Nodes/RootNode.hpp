#pragma once

#include "Core/Utility/Singleton.hpp"
#include "Node.hpp"

namespace mm
{

class RootNode : public Node, public Singleton<RootNode>
{
public:
	virtual void Startup() override;
	virtual void Shutdown() override;

private:
};

}

