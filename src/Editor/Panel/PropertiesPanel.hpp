#pragma once

#include "Panel.hpp"

namespace mm
{

class Node;

class PropertiesPanel : public Panel
{
public:
	virtual void OnUpdate(f32 deltaTime) override;
	virtual void OnRender() override;

private:
	void Visit(Node& node);
};
}