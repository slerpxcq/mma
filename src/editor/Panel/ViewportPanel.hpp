#pragma once

#include "Panel.hpp"

namespace mm
{

class ViewportPanel : public Panel
{
public:
	virtual void OnUpdate(f32 deltaTime) override; 
	virtual void OnRender() override;

private:
};

}

