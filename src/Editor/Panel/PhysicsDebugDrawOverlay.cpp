#include "EditorPch.hpp"
#include "PhysicsDebugDrawOverlay.hpp"

#include "Core/Physics/PhysicsManager.hpp"

namespace mm
{

void PhysicsDebugDrawOverlay::OnUpdate(f32 deltaTime)
{
}

void PhysicsDebugDrawOverlay::OnRender()
{
	GetPhysicsManager()->DebugDrawWorld();
}

}


