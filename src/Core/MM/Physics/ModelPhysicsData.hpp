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
			BT_KINEMATIC = 1 << 0,
			BT_DYNAMIC = 1 << 1,
			BT_ALL = BT_KINEMATIC | BT_DYNAMIC
		};

	public:
		ModelPhysicsData(PhysicsWorld& world, Model& model);
		~ModelPhysicsData();
		void LoadTransforms(uint32_t bodyType, bool bind);
	
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

