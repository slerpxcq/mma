#pragma once

#include "Core/Utility/Singleton.hpp"

#include <btBulletDynamicsCommon.h>

namespace mm
{

class PhysicsManager : public Singleton<PhysicsManager>
{
public:
	virtual void Startup() override;
	virtual void Shutdown() override;

private:
	std::unique_ptr<btDefaultCollisionConfiguration>     m_collisionConfig;
	std::unique_ptr<btCollisionDispatcher>               m_dispatcher;
	std::unique_ptr<btBroadphaseInterface>               m_pairCache;
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
	std::unique_ptr<btDiscreteDynamicsWorld>             m_dynamicsWorld;

	bool m_enabled = false;
};

}

