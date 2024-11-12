#pragma once

#include "PanelOverlay.hpp"

namespace mm
{

class Panel;
class CameraControllerOverlay : public PanelOverlay
{
public:
	CameraControllerOverlay(Panel& parent, StringView name);

	void OnUpdate(f32) override;
	void OnRender() override;
};

}

