#include "mmpch.hpp"
#include "PhysicsManager.hpp"

namespace mm
{
void PhysicsManager::Startup()
{
	m_collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
	m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfig.get());
	m_pairCache = std::make_unique<btDbvtBroadphase>();
	m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
	m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
		m_dispatcher.get(), m_pairCache.get(), m_solver.get(), m_collisionConfig.get());

	m_dynamicsWorld->setGravity(btVector3(0, -98, 0));
}

void PhysicsManager::Shutdown()
{
}

}


