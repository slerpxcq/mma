#include "mmpch.hpp"
#include "PhysicsWorld.hpp"

#include "Core/MM/Files/PMXFile.hpp"
#include "Core/Utility/Type.hpp"

namespace mm
{
	PhysicsWorld::PhysicsWorld() 
	{
	//	m_debugDrawer = std::make_unique<DebugDrawer>();
		m_collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
		m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfig.get());
		m_pairCache = std::make_unique<btDbvtBroadphase>();
		m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
		m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
			m_dispatcher.get(), m_pairCache.get(), m_solver.get(), m_collisionConfig.get());

		m_dynamicsWorld->setGravity(btVector3(0, -98, 0));
	//	m_debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawConstraints);
	//	m_dynamicsWorld->setDebugDrawer(m_debugDrawer.get());
	}

	PhysicsWorld::~PhysicsWorld()
	{
	}

	void PhysicsWorld::Update(float deltaTime)
	{
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
		for (auto& model : m_models)
			model->LoadTransforms(ModelPhysicsData::BT_ALL, true);
	}

	void PhysicsWorld::LoadTransforms()
	{
		for (auto& model : m_models)
			model->LoadTransforms(ModelPhysicsData::BT_KINEMATIC, false);
	}

	ModelPhysicsData* PhysicsWorld::LoadModel(Model& model)
	{
		m_models.push_back(std::make_unique<ModelPhysicsData>(*this, model));
		return m_models.back().get();
	}
}
