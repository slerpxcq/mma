#include "CorePch.hpp"
#include "PhysicsManager_Bullet.hpp"

#include "Rigidbody_Bullet.hpp"

#include "SceneManager.hpp"

namespace mm
{

PhysicsManager_Bullet::PhysicsManager_Bullet()
{
}

Rigidbody* PhysicsManager_Bullet::CreateRigidbody(const Rigidbody::ConstructInfo& info)
{
	btVector3 localInertia;
	btScalar mass;
	if (info.isDynamic) {
		auto shape = static_cast<btCollisionShape*>(info.collider.GetHandle());
		shape->calculateLocalInertia(info.mass, localInertia);
		mass = info.mass;
	} else {
		mass = 0;
	}
	btTransform bindWorld = Cast<btTransform>(info.bindWorld);
	m_bindTransforms.push_back(bindWorld);
	m_motionStates.push_back(MakeScoped<btDefaultMotionState>(bindWorld));
	auto info2 = btRigidBody::btRigidBodyConstructionInfo(mass,
														  m_motionStates.back().get(),
														  m_collisionShapes.back().get(),
														  localInertia);
	auto rigidbody = MakeScoped<btRigidBody>(info2);
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
	m_dynamicsWorld->addRigidBody(rigidbody.get(),
								  1 << info.group,
								  info.noCollisionGroupMask);
}

Collider PhysicsManager_Bullet::CreateCollider(const Collider::ConstructInfo& info)
{
	switch (info.type) {
	case Collider::ConstructInfo::BOX:
		m_collisionShapes.push_back(MakeScoped<btBoxShape>(info.box.width,
												           info.box.height,
												           info.box.depth));
		break;
	case Collider::ConstructInfo::SPHERE:
		m_collisionShapes.push_back(MakeScoped<btCapsuleShape>(info.capsule.radius,
													           info.capsule.height));
		break;
	case Collider::ConstructInfo::CAPSULE:
		m_collisionShapes.push_back(MakeScoped<btSphereShape>(info.sphere.radius));
		break;
	default:
		MM_CORE_UNREACHABLE();
	}

	Collider result;
	result.SetHandle(static_cast<void*>(m_collisionShapes.back().get()));
	return result;
}

}
