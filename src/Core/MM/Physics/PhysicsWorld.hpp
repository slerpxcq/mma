#pragma once

#include "ModelPhysicsData.hpp"

#include <btBulletDynamicsCommon.h>

namespace mm
{
	class PMXFile;

	class PhysicsWorld
	{
		friend class ModelPhysicsData;
		static constexpr float MIN_STEP = 1.f / 60;
	public:
		PhysicsWorld();
		~PhysicsWorld();
		void Update(float deltaTime);
		void Reset();
		void LoadTransforms();
		void SetEnable(bool value) { 
			if (value == true) 
				Reset();
			m_enabled = value;
		}
		bool IsEnabled() { return m_enabled; }

		ModelPhysicsData* LoadModel(Model& model);

	private:
		std::unique_ptr<btDefaultCollisionConfiguration>     m_collisionConfig;
		std::unique_ptr<btCollisionDispatcher>               m_dispatcher;
		std::unique_ptr<btBroadphaseInterface>               m_pairCache;
		std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
		std::unique_ptr<btDiscreteDynamicsWorld>             m_dynamicsWorld;

		// Per model data
		std::vector<std::unique_ptr<ModelPhysicsData>> m_models;

		bool m_enabled = false;
	};
}


