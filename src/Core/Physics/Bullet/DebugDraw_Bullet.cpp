#include "CorePch.hpp"
#include "DebugDraw_Bullet.hpp"

#include "../PhysicsManager.hpp"

namespace mm
{

void DebugDraw_Bullet::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	auto col = Color{ Cast<Vec3>(color), 1 };
	GetPhysicsManager()->GetDebugDraw()->DrawLine(Cast<Vec3>(from), Cast<Vec3>(to), col);
}

void DebugDraw_Bullet::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	// TO BE IMPLEMENTED
}

void DebugDraw_Bullet::reportErrorWarning(const char* warningString)
{
	// TO BE IMPLEMENTED
}

void DebugDraw_Bullet::draw3dText(const btVector3& location, const char* textString)
{
	// TO BE IMPLEMENTED
}

void DebugDraw_Bullet::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

int DebugDraw_Bullet::getDebugMode() const
{
	return m_debugMode;
}

}
