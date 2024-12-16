#include "CorePch.hpp"
#include "PhysicsManager_Bullet.hpp"

#include "DebugDraw_Bullet.hpp"
#include "SceneManager.hpp"

namespace mm
{

PhysicsManager_Bullet::PhysicsManager_Bullet() :
	m_collisionConfig{ MakeScoped<btDefaultCollisionConfiguration>() },
	m_dispatcher{ MakeScoped<btCollisionDispatcher>(m_collisionConfig.get()) },
	m_pairCache{ MakeScoped<btDbvtBroadphase>() },
	m_solver{ MakeScoped<btSequentialImpulseConstraintSolver>() },
	m_dynamicsWorld{ MakeScoped<btDiscreteDynamicsWorld>(m_dispatcher.get(), 
														 m_pairCache.get(), 
														 m_solver.get(), 
														 m_collisionConfig.get()) },
	m_debugDraw{ MakeScoped<DebugDraw_Bullet>() } 
{
	m_dynamicsWorld->setGravity({ 0, -98, 0 });
	m_dynamicsWorld->setDebugDrawer(m_debugDraw.get());
}

PhysicsManager_Bullet::~PhysicsManager_Bullet()
{
	for (auto&& constraint : m_constraints) {
		m_dynamicsWorld->removeConstraint(constraint.get());
	}
	for (auto&& rigidbody : m_rigidbodies) {
		m_dynamicsWorld->removeRigidBody(rigidbody.get());
	}
}

void PhysicsManager_Bullet::StepSimulation(f32 deltaTime)
{
	static constexpr f32 MIN_STEP = 1.f / 60;
	static f32 acc;

	m_dynamicsWorld->stepSimulation(deltaTime);

	// acc += deltaTime;
	// while (acc >= MIN_STEP) {
	// 	m_dynamicsWorld->stepSimulation(MIN_STEP);
	// 	acc -= MIN_STEP;
	// }
}

Rigidbody* PhysicsManager_Bullet::CreateRigidbody(const Rigidbody::ConstructInfo& info)
{
	auto shape = static_cast<btCollisionShape*>(info.collider.GetHandle());
	btVector3 localInertia(0, 0, 0);
	btScalar mass = 0;
	switch (info.type) {
	case Rigidbody::Type::KINEMATIC:
		mass = 0;
		break;
	case Rigidbody::Type::DYNAMIC:
	case Rigidbody::Type::DYNAMIC_FOLLOW:
		shape->calculateLocalInertia(info.mass, localInertia);
		mass = info.mass;
		break;
	default:
		MM_CORE_UNREACHABLE();
	}
	auto motionState = MakeScoped<btDefaultMotionState>(Cast<btTransform>(info.bindWorld));
	auto info2 = btRigidBody::btRigidBodyConstructionInfo(mass,
														  motionState.get(),
														  shape,
														  localInertia);
	m_motionStates.push_back(std::move(motionState));
	auto rigidbody = MakeScoped<btRigidBody>(info2);
	rigidbody->setDamping(info.linearDamping, info.angularDamping);
	rigidbody->setFriction(info.friction);
	rigidbody->setRestitution(info.restitution);
	rigidbody->setActivationState(DISABLE_DEACTIVATION);
	if (info.type == Rigidbody::Type::KINEMATIC) {
		rigidbody->setCollisionFlags(rigidbody->getCollisionFlags() | 
									 btCollisionObject::CF_KINEMATIC_OBJECT);
	}
	m_dynamicsWorld->addRigidBody(rigidbody.get(),
								  1 << info.group,
								  info.noCollisionGroupMask);
	auto sm = GetSceneManager();
	Rigidbody::Builder builder{ info };
	builder.SetBindWorld(info.bindWorld);
	builder.SetType(info.type);
	builder.SetHandle(rigidbody.get());
	builder.SetCollider(info.collider);
	builder.SetBone(info.bone);
	m_rigidbodies.push_back(move(rigidbody));
	return builder.Get();
}

Collider PhysicsManager_Bullet::CreateCollider(const Collider::ConstructInfo& info)
{
	switch (info.type) {
	case Collider::ConstructInfo::BOX:
		m_collisionShapes.push_back(MakeScoped<btBoxShape>(Cast<btVector3>(info.data.box.size)));
		break;
	case Collider::ConstructInfo::SPHERE:
		m_collisionShapes.push_back(MakeScoped<btSphereShape>(info.data.sphere.radius));
		break;
	case Collider::ConstructInfo::CAPSULE:
		m_collisionShapes.push_back(MakeScoped<btCapsuleShape>(info.data.capsule.radius,
															   info.data.capsule.height));
		break;
	default:
		MM_CORE_UNREACHABLE();
	}

	Collider result;
	result.SetHandle(m_collisionShapes.back().get());
	return result;
}

Constraint PhysicsManager_Bullet::CreateConstraint(const Constraint::ConstructInfo& info)
{
	switch (info.type) {
	case Constraint::Type::GENERIC_6DOF_SPRING: {
		Transform frameInA = info.rigidbodyA->GetBindWorld().Inverse() * info.bindWorld;
		Transform frameInB = info.rigidbodyB->GetBindWorld().Inverse() * info.bindWorld;
		auto constraint = MakeScoped<btGeneric6DofSpring2Constraint>(
			*static_cast<btRigidBody*>(info.rigidbodyA->GetHandle()),
			*static_cast<btRigidBody*>(info.rigidbodyB->GetHandle()),
			Cast<btTransform>(frameInA),
			Cast<btTransform>(frameInB));
		for (i32 i = 0; i < 3; ++i) {
			constraint->setLimit(i, info.linearLimit.first[i], info.linearLimit.second[i]);
			constraint->setStiffness(i, info.linearStiffness[i]);
			constraint->enableSpring(i, true);
			constraint->setLimit(i+3, info.angularLimit.first[i], info.angularLimit.second[i]);
			constraint->setStiffness(i+3, info.angularStiffness[i]);
			constraint->enableSpring(i+3, true);
		}
		constraint->setDbgDrawSize(1.f);
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

void PhysicsManager_Bullet::PullRigidbodyTransform(Rigidbody* body)
{
	auto rigidbody = static_cast<btRigidBody*>(body->GetHandle());
	btTransform transform;
	rigidbody->getMotionState()->getWorldTransform(transform);
	body->GetNode()->SetWorldTransform(Cast<Transform>(transform));
}

void PhysicsManager_Bullet::PushRigidbodyTransform(Rigidbody* body, 
												   const Transform& transform, 
												   Transform::Type type)
{
	auto rigidbody = static_cast<btRigidBody*>(body->GetHandle());
	auto motionState = rigidbody->getMotionState();
	btTransform xfm;
	motionState->getWorldTransform(xfm);
	if (type & Transform::Type::ROTATION_BIT) {
		xfm.setRotation(Cast<btQuaternion>(transform.rotation));
	}
	if (type & Transform::Type::TRANSLATION_BIT) {
		xfm.setOrigin(Cast<btVector3>(transform.translation));
	}
	rigidbody->clearForces();
	rigidbody->setLinearVelocity(btVector3(0, 0, 0));
	rigidbody->setAngularVelocity(btVector3(0, 0, 0));
	rigidbody->setWorldTransform(xfm);
	motionState->setWorldTransform(xfm);
}

void PhysicsManager_Bullet::DebugDrawWorld() const
{
	for (auto&& rigidbody : m_rigidbodies) {
		btVector3 color = 
			(rigidbody->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT) ?
			btVector3(1, 0, 0) : btVector3(1, 1, 0);
		btTransform transform;
		rigidbody->getMotionState()->getWorldTransform(transform);
		m_dynamicsWorld->debugDrawObject(transform,
										 rigidbody->getCollisionShape(),
										 color);
	}
	for (auto&& constraint : m_constraints) {
		m_dynamicsWorld->debugDrawConstraint(constraint.get());
	}
}

}
