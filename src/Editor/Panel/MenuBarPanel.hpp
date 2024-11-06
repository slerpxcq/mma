#pragma once

#include "Panel.hpp"

namespace mm
{

class MenuBarPanel : public Panel
{
public: 
	MenuBarPanel(StringView name = "MenuBar") : Panel{name} {} 
	virtual void OnUpdate(f32 deltaTime) override;
	virtual void OnRender() override;
};

}

