#pragma once

#include "../PhysicsManager.hpp"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace mm
{

class PhysicsManager_Bullet : public PhysicsManager
{
public:
	PhysicsManager_Bullet();
	virtual Rigidbody* CreateRigidbody(const Rigidbody::ConstructInfo& info) override;
	virtual Collider CreateCollider(const Collider::ConstructInfo& info) override;
	virtual Constraint CreateConstraint(const Constraint::ConstructInfo& info) override;

private:
	Scoped<btDefaultCollisionConfiguration>     m_collisionConfig;
	Scoped<btCollisionDispatcher>               m_dispatcher;
	Scoped<btBroadphaseInterface>               m_pairCache;
	Scoped<btSequentialImpulseConstraintSolver> m_solver;
	Scoped<btDiscreteDynamicsWorld>             m_dynamicsWorld;

	DynArray<Scoped<btCollisionShape>>          m_collisionShapes;
	DynArray<Scoped<btRigidBody>>               m_rigidbodies;
	DynArray<Scoped<btMotionState>>             m_motionStates;
	DynArray<Scoped<btTypedConstraint>>         m_constraints;
};

}

