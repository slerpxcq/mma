#include "mmpch.hpp"
#include "PhysicsWorld.hpp"

#include "Core/MM/Files/PMXFile.hpp"
#include "Core/Utility/Type.hpp"

#include "Core/MM/Model/Model.hpp"

namespace mm
{
	PhysicsWorld::PhysicsWorld() 
	{
		m_collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
		m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfig.get());
		m_pairCache = std::make_unique<btDbvtBroadphase>();
		m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
		m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
			m_dispatcher.get(), m_pairCache.get(), m_solver.get(), m_collisionConfig.get());

		m_dynamicsWorld->setGravity(btVector3(0, -98, 0));
	}

	PhysicsWorld::~PhysicsWorld()
	{
	}

	void PhysicsWorld::Update(float deltaTime)
	{
		if (!m_enabled)
			return;

		static float accum = 0.0f;

		accum += deltaTime;
		while (accum >= MIN_STEP) {
			LoadTransforms();
			m_dynamicsWorld->stepSimulation(MIN_STEP);
			accum -= MIN_STEP;
		}
	}

	void PhysicsWorld::Reset()
	{
		for (auto& physicsData : m_models) {
			physicsData->GetModel().GetArmature().UpdatePose();
			physicsData->LoadTransforms(ModelPhysicsData::BODY_TYPE_ALL);
		}
	}

	void PhysicsWorld::LoadTransforms()
	{
		for (auto& model : m_models)
			model->LoadTransforms(ModelPhysicsData::BODY_TYPE_KINEMATIC);
	}

	ModelPhysicsData* PhysicsWorld::LoadModel(Model& model)
	{
		m_models.push_back(std::make_unique<ModelPhysicsData>(*this, model));
		return m_models.back().get();
	}
}
