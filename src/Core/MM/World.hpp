#pragma once

#include "Core/MM/Camera/Camera.hpp"
#include "Core/MM/Model/Model.hpp"
#include "Core/MM/Physics/PhysicsWorld.hpp"

namespace mm
{
	class GLRenderer;
	class World
	{
		using ModelContainer = std::vector<std::unique_ptr<Model>>;
	public:
		World();
		void OnUpdate(float deltaTime);
		Model* LoadModel(const std::filesystem::path& path);
		void Render(GLRenderer&);
		const Camera& GetCamera() const { return m_mainCamera; }
		Camera& GetCamera() { return m_mainCamera; }

		PhysicsWorld& GetPhysicsWorld() { return m_physicsWorld; }
		const ModelContainer& GetModels() const { return m_models; }

	private:
		Camera m_mainCamera;
		ModelContainer m_models;
		PhysicsWorld m_physicsWorld;
	};
}

