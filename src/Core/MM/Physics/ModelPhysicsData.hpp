#pragma once

#include <btBulletDynamicsCommon.h>

namespace mm
{
	class Model;
	class PhysicsWorld;

	class ModelPhysicsData
	{
		friend class Armature;
	public:
		enum BodyType {
			BODY_TYPE_KINEMATIC = 1 << 0,
			BODY_TYPE_DYNAMIC = 1 << 1,
			BODY_TYPE_ALL = BODY_TYPE_KINEMATIC | BODY_TYPE_DYNAMIC
		};

	public:
		ModelPhysicsData(PhysicsWorld& world, Model& model);
		~ModelPhysicsData();
		Model& GetModel() { return m_model; }
		void LoadTransforms(uint32_t bodyType);
	
	private:
		void LoadRigidbodies();
		void LoadConstraints();
		void ResetRigidbody(btRigidBody& body, const btTransform& transform);
		void ResetMotionState(btMotionState& state, const btTransform& transform);

	private:
		PhysicsWorld& m_world;
		Model& m_model;
		std::vector<std::unique_ptr<btCollisionShape>>       m_collisionShapes;
		std::vector<std::unique_ptr<btRigidBody>>            m_rigidbodies;
		std::vector<std::unique_ptr<btMotionState>>          m_motionStates;
		std::vector<btTransform>                             m_bindTransforms;
		std::vector<std::unique_ptr<btTypedConstraint>>      m_constraints;
	};
}

