#include "EditorPch.hpp"
#include "PhysicsDebugDrawOverlay.hpp"

#include "ViewportPanel.hpp"

#include "Core/Physics/PhysicsManager.hpp"

namespace mm
{

PhysicsDebugDrawOverlay::PhysicsDebugDrawOverlay(Panel& parent) :
	PanelOverlay{ parent } 
{
	MM_APP_ASSERT(dynamic_cast<ViewportPanel*>(&parent));
	m_debugDraw = MakeScoped<DebugDrawImpl_ImGui>(*this);
}

void PhysicsDebugDrawOverlay::OnUpdate(f32 deltaTime)
{
}

void PhysicsDebugDrawOverlay::OnRender()
{
	//GetPhysicsManager()->DebugDrawWorld();
}

}


