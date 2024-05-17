#include "mmpch.hpp"
#include "ModelPhysicsData.hpp"

#include "PhysicsWorld.hpp"

#include "Core/MM/Model/Model.hpp"
#include "Core/Utility/Type.hpp"

namespace mm
{
	ModelPhysicsData::ModelPhysicsData(PhysicsWorld& world, Model& model) :
		m_world(world),
		m_model(model)
	{
		LoadRigidbodies();
		LoadConstraints();
	}

	ModelPhysicsData::~ModelPhysicsData()
	{
		for (auto& c : m_constraints)
			m_world.m_dynamicsWorld->removeConstraint(c.get());

		for (auto& b : m_rigidbodies)
			m_world.m_dynamicsWorld->removeRigidBody(b.get());
	}

	void ModelPhysicsData::LoadRigidbodies()
	{
		const auto& pmxRigidbodies = m_model.GetPMXFile().GetRigidbodies();
		std::string name;
		for (uint32_t i = 0; i < pmxRigidbodies.size(); ++i) {
			const auto& pmxRigidbody = pmxRigidbodies[i];
			name = pmxRigidbody.nameJP;

			switch (pmxRigidbody.shape) {
				case PMXFile::RB_BOX:
				{
					m_collisionShapes.push_back(std::make_unique<btBoxShape>(
						btVec3FromArray(pmxRigidbody.size)));
					break;
				}
				case PMXFile::RB_CAPSULE:
				{
					m_collisionShapes.push_back(std::make_unique<btCapsuleShape>(
						pmxRigidbody.size[0], pmxRigidbody.size[1]));
					break;
				}
				case PMXFile::RB_SPHERE:
				{
					m_collisionShapes.push_back(std::make_unique<btSphereShape>(
						pmxRigidbody.size[0]));
					break;
				}
			}

			btScalar mass(pmxRigidbody.mass);

			btVector3 localInertia(0, 0, 0);
			bool isDynamic = pmxRigidbody.physicsType == PMXFile::RB_DYNAMIC;
			if (isDynamic)
				m_collisionShapes.back()->calculateLocalInertia(mass, localInertia);
			else
				mass = 0;

			m_bindTransforms.emplace_back();
			btQuaternion q;
			btVector3 euler(btEulerFromArray(pmxRigidbody.rotation));
			q.setEuler(euler.getX(), euler.getY(), euler.getZ());
			btTransform& transform = m_bindTransforms.back();
			transform.setOrigin(btVec3FromArray(pmxRigidbody.position));
			transform.setRotation(q);

			m_motionStates.push_back(std::make_unique<btDefaultMotionState>(transform));

			m_rigidbodies.push_back(std::make_unique<btRigidBody>(
				btRigidBody::btRigidBodyConstructionInfo(
					mass,
					m_motionStates.back().get(), 
					m_collisionShapes.back().get(),
					localInertia)));

			auto& rigidbody = m_rigidbodies.back();
			rigidbody->setDamping(pmxRigidbody.linearDamping, pmxRigidbody.angularDamping);
			rigidbody->setFriction(pmxRigidbody.friction);
			rigidbody->setRestitution(pmxRigidbody.restitution);
			rigidbody->setActivationState(DISABLE_DEACTIVATION);

			if (isDynamic) {
				rigidbody->setCollisionFlags(rigidbody->getCollisionFlags() | btCollisionObject::CF_DYNAMIC_OBJECT);
			}
			else {
				rigidbody->setCollisionFlags(rigidbody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			}

			m_world.m_dynamicsWorld->addRigidBody(rigidbody.get(), 1 << pmxRigidbody.group, pmxRigidbody.noCollisionGroup);
			MM_INFO("{0}: rigidbody loaded", name);
		}
	}

	void ModelPhysicsData::LoadConstraints()
	{
		const auto& pmxJoints = m_model.GetPMXFile().GetJoints();
		std::string name;

		for (uint32_t i = 0; i < pmxJoints.size(); ++i) {
			const auto& pmxJoint = pmxJoints[i];
			name = pmxJoint.nameJP;
			switch (pmxJoint.type) {
			case PMXFile::JOINT_GENERIC_6DOF_SPRING:
			{
				btTransform jointTransform;
				btQuaternion q;

				jointTransform.setOrigin(btVec3FromArray(pmxJoint.position));
				btVector3 euler = btEulerFromArray(pmxJoint.rotation);
				q.setEuler(euler.getX(), euler.getY(), euler.getZ());
				jointTransform.setRotation(q);

				int32_t indexA = pmxJoint.rigidbodyIndices[0];
				int32_t indexB = pmxJoint.rigidbodyIndices[1];
				const btTransform& transformA = m_bindTransforms[indexA];
				const btTransform& transformB = m_bindTransforms[indexB];
				btRigidBody& rigidbodyA = *m_rigidbodies[indexA];
				btRigidBody& rigidbodyB = *m_rigidbodies[indexB];

				btTransform localFrameA;
				btVector3 worldOffset = jointTransform.getOrigin() - transformA.getOrigin();
				q = transformA.getRotation().inverse();
				localFrameA.setOrigin(worldOffset.rotate(q.getAxis(), q.getAngle()));
				localFrameA.setRotation(q * jointTransform.getRotation());

				btTransform localFrameB;
				worldOffset = jointTransform.getOrigin() - transformB.getOrigin();
				q = transformB.getRotation().inverse();
				localFrameB.setOrigin(worldOffset.rotate(q.getAxis(), q.getAngle()));
				localFrameB.setRotation(q * jointTransform.getRotation());

				std::unique_ptr<btGeneric6DofSpring2Constraint> constraint =
					std::make_unique<btGeneric6DofSpring2Constraint>(
						rigidbodyA, rigidbodyB,
						localFrameA, localFrameB);

				for (uint32_t j = 0; j < 6; ++j) {
					if (j < 3) {
						constraint->setLimit(j, pmxJoint.linearLimit[0][j], pmxJoint.linearLimit[1][j]);
						constraint->setStiffness(j, pmxJoint.linearStiffness[j]);
					}
					else {
						uint32_t k = j - 3;
						constraint->setLimit(j, pmxJoint.angularLimit[0][k], pmxJoint.angularLimit[1][k]);
						constraint->setStiffness(j, pmxJoint.angularStiffness[k]);
					}
					constraint->enableSpring(j, true);
				}

				constraint->setDbgDrawSize(1.f);

				m_world.m_dynamicsWorld->addConstraint(constraint.get(), true);

				m_constraints.push_back(std::move(constraint));
				break;
			}
			default:
				MM_ASSERT(0 && "Unsupported joint type");
				break;
			}

			MM_INFO("{0}: joint loaded", name);
		}
	}

	void ModelPhysicsData::LoadTransforms(uint32_t bodyType)
	{
		const auto& pmxRigidbodies = m_model.GetPMXFile().GetRigidbodies();

		for (uint32_t i = 0; i < pmxRigidbodies.size(); ++i) {
			const auto& pmxRigidbody = pmxRigidbodies[i];
			bool isKinematic = pmxRigidbody.physicsType == PMXFile::RB_KINEMATIC;
			bool isDynamic = pmxRigidbody.physicsType == PMXFile::RB_DYNAMIC;
			if (((bodyType & BODY_TYPE_KINEMATIC) && isKinematic) || ((bodyType & BODY_TYPE_DYNAMIC) && isDynamic)) {
				const auto& bone = m_model.GetArmature().GetBones()[pmxRigidbody.boneIndex];
				const btTransform& bindTransform = m_bindTransforms[i];
				btVector3 bindOffset = bindTransform.getOrigin() - btVec3FromGLM(bone.bindWorldInv.inverse().translation);

				btTransform transform;
				btQuaternion rotation;
				btVector3 translation;
				rotation = btQuatFromGLM(bone.animWorld.rotation);
				translation = btVec3FromGLM(bone.animWorld.translation);
				transform.setOrigin(translation + bindOffset.rotate(rotation.getAxis(), rotation.getAngle()));
				transform.setRotation(rotation * bindTransform.getRotation());

				ResetRigidbody(*m_rigidbodies[i], transform);
				ResetMotionState(*m_motionStates[i], transform);
			}
		}
	}

	void ModelPhysicsData::ResetRigidbody(btRigidBody& body, const btTransform& transform)
	{
		body.clearForces();
		body.setLinearVelocity(btVector3(0, 0, 0));
		body.setAngularVelocity(btVector3(0, 0, 0));
		body.setWorldTransform(transform);
	}

	void ModelPhysicsData::ResetMotionState(btMotionState& state, const btTransform& transform)
	{
		state.setWorldTransform(transform);
	}
}

