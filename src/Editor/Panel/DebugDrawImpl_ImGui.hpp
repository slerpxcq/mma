#pragma once

#include "Core/Physics/DebugDraw.hpp"

namespace mm
{

class PhysicsDebugDrawOverlay;
class DebugDrawImpl_ImGui : public DebugDraw
{
public:
	DebugDrawImpl_ImGui(PhysicsDebugDrawOverlay& parent) :
		m_parent{ parent } {}
	virtual void DrawLine(const Vec3& from, const Vec3& to, const Color& color) const override;

private:
	PhysicsDebugDrawOverlay& m_parent;
};

}

