#pragma once

#include "PanelOverlay.hpp"

namespace mm
{

class PoseEditorOverlay : public PanelOverlay
{
public:
	PoseEditorOverlay(Panel& parent, StringView name);

	void OnUpdate(f32) override;
	void OnRender() override;

private:
	Vec3 ToScreenPos(Vec3 pos);

private:
	static constexpr f32 BUTTON_RADIUS = 7.5f;
	static constexpr u32 OUTLINE_COLOR = 0x7fc0c0c0;
	static constexpr u32 FILL_COLOR = 0x3fc0c0c0;
	static constexpr f32 OUTLINE_SIZE = 1.5f;

};

}

