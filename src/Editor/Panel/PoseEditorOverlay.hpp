#pragma once

#include "PanelOverlay.hpp"

namespace mm
{

class PoseEditorOverlay : public PanelOverlay
{
public:
	PoseEditorOverlay(Panel& parent, StringView name) :
		PanelOverlay{ parent, name } {}

	void OnUpdate(f32) override;
	void OnRender() override;

private:
};

}

