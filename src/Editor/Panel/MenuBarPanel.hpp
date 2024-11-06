#pragma once

#include "Panel.hpp"

namespace mm
{

class MenuBarPanel : public Panel
{
public: 
	MenuBarPanel() : Panel{ "Menu Bar" } {}
	virtual void OnUpdate(f32 deltaTime) override;
	virtual void OnRender() override;
};

}

