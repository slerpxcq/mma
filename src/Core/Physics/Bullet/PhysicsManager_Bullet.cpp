#include "CorePch.hpp"
#include "PhysicsManager_Bullet.hpp"

#include "SceneManager.hpp"

namespace mm
{

PhysicsManager_Bullet::PhysicsManager_Bullet()
{
	m_collisionConfig = MakeScoped<btDefaultCollisionConfiguration>();
	m_dispatcher = MakeScoped<btCollisionDispatcher>(m_collisionConfig.get());
	m_pairCache = MakeScoped<btDbvtBroadphase>();
	m_solver = MakeScoped<btSequentialImpulseConstraintSolver>();
	m_dynamicsWorld = MakeScoped<btDiscreteDynamicsWorld>(
	m_dispatcher.get(), m_pairCache.get(), m_solver.get(), m_collisionConfig.get());
	m_dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
}

Rigidbody* PhysicsManager_Bullet::CreateRigidbody(const Rigidbody::ConstructInfo& info)
{
	btVector3 localInertia(0, 0, 0);
	btScalar mass = 0;
	if (info.isDynamic) {
		auto shape = static_cast<btCollisionShape*>(info.collider.GetHandle());
		shape->calculateLocalInertia(info.mass, localInertia);
		mass = info.mass;
	} else {
		mass = 0;
	}
	m_motionStates.push_back(MakeScoped<btDefaultMotionState>(Cast<btTransform>(info.bindWorld)));
	auto info2 = btRigidBody::btRigidBodyConstructionInfo(mass,
														  m_motionStates.back().get(),
														  m_collisionShapes.back().get(),
														  localInertia);
	m_rigidbodies.push_back(MakeScoped<btRigidBody>(info2));
	auto rigidbody = m_rigidbodies.back().get();
	rigidbody->setDamping(info.linearDamping, info.angularDamping);
	rigidbody->setFriction(info.friction);
	rigidbody->setRestitution(info.restitution);
	rigidbody->setActivationState(DISABLE_DEACTIVATION);
	if (info.isDynamic) {
		rigidbody->setCollisionFlags(rigidbody->getCollisionFlags() | 
									 btCollisionObject::CF_DYNAMIC_OBJECT);
	} else {
		rigidbody->setCollisionFlags(rigidbody->getCollisionFlags() | 
									 btCollisionObject::CF_KINEMATIC_OBJECT);
	}
	m_dynamicsWorld->addRigidBody(rigidbody,
								  1 << info.group,
								  info.noCollisionGroupMask);
	auto sm = GetSceneManager();
	auto result = sm->CreateObject<Rigidbody>(info.name);
	result->SetBindWorld(info.bindWorld);
	result->SetHandle(rigidbody);
	return result;
}

Collider PhysicsManager_Bullet::CreateCollider(const Collider::ConstructInfo& info)
{
	switch (info.type) {
	case Collider::ConstructInfo::BOX:
		m_collisionShapes.push_back(MakeScoped<btBoxShape>(Cast<btVector3>(info.data.box.size)));
		break;
	case Collider::ConstructInfo::SPHERE:
		m_collisionShapes.push_back(MakeScoped<btCapsuleShape>(info.data.capsule.radius,
													           info.data.capsule.height));
		break;
	case Collider::ConstructInfo::CAPSULE:
		m_collisionShapes.push_back(MakeScoped<btSphereShape>(info.data.sphere.radius));
		break;
	default:
		MM_CORE_UNREACHABLE();
	}

	Collider result;
	result.SetHandle(m_collisionShapes.back().get());
	return result;
}

static Transform ComputeLocalFrame(const Transform& jointBindWorld, const Transform& rigidbodyBindWorld)
{
	return jointBindWorld.Inverse() * rigidbodyBindWorld;
}

Constraint PhysicsManager_Bullet::CreateConstraint(const Constraint::ConstructInfo& info)
{
	switch (info.type) {
	case Constraint::Type::GENERIC_6DOP_SPRING: {
		Transform localFrameA = info.bindWorld.Inverse() * info.rigidbodyA->GetBindWorld();
		Transform localFrameB = info.bindWorld.Inverse() * info.rigidbodyB->GetBindWorld();
		auto constraint = MakeScoped<btGeneric6DofSpring2Constraint>(
			*static_cast<btRigidBody*>(info.rigidbodyA->GetHandle()),
			*static_cast<btRigidBody*>(info.rigidbodyB->GetHandle()),
			Cast<btTransform>(localFrameA),
			Cast<btTransform>(localFrameB));
		for (i32 i = 0; i < 3; ++i) {
			constraint->setLimit(i, info.linearLimit.first[i], info.linearLimit.second[i]);
			constraint->setStiffness(i, info.linearStiffness[i]);
			constraint->setLimit(i+3, info.angularLimit.first[i], info.angularLimit.second[i]);
			constraint->setStiffness(i+3, info.angularStiffness[i]);
			constraint->enableSpring(i, true);
			constraint->enableSpring(i+3, true);
		}
		m_dynamicsWorld->addConstraint(constraint.get(), true);
		m_constraints.push_back(std::move(constraint)); }
		break;
	default:
		MM_CORE_UNREACHABLE();
	}

	Constraint result{ info.name };
	result.SetHandle(m_constraints.back().get());
	return result;
}

}
