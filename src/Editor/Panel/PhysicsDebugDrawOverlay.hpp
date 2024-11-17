#pragma once

#include "PanelOverlay.hpp"
#include "DebugDrawImpl_ImGui.hpp"

namespace mm
{

class PhysicsDebugDrawOverlay : public PanelOverlay
{
public:
	PhysicsDebugDrawOverlay(Panel& parent);

	virtual void OnUpdate(f32) override;
	virtual void OnRender() override;

	DebugDraw* GetDebugDraw() const { return m_debugDraw.get(); }

private:
	Scoped<DebugDraw> m_debugDraw{};
};

}

