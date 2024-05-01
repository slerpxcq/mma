#pragma once

#include "Core/MM/Camera/Camera.hpp"
#include "Core/MM/Model/Model.hpp"
#include "Core/MM/Physics/PhysicsWorld.hpp"

namespace mm
{
	class Renderer;
	class World
	{
		friend class WorldSerDes;

		using ModelContainer = std::vector<std::unique_ptr<Model>>;
	public:
		World();
		~World();
		void OnUpdate(float deltaTime);
		Model* LoadModel(const std::filesystem::path& path);
		void Render(Renderer&);
		const Camera& GetCamera() const { return m_mainCamera; }
		Camera& GetCamera() { return m_mainCamera; }

		PhysicsWorld& GetPhysicsWorld() { return m_physicsWorld; }
		const ModelContainer& GetModels() const { return m_models; }
		ModelContainer& GetModels() { return m_models; }

	private:
		Camera m_mainCamera;
		ModelContainer m_models;
		//std::vector<std::unique_ptr<Light>> m_lights;
		PhysicsWorld m_physicsWorld;
	};
}

