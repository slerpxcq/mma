#pragma once

#include "Core/Camera.hpp"
#include "Core/MM/Model.hpp"
#include "Core/MM/Physics/PhysicsWorld.hpp"

namespace mm
{
	class GLRenderer;
	class World
	{
		using ModelContainer = std::map<std::string, std::unique_ptr<Model>>;
	public:
		World();
		void OnUpdate(float deltaTime);
		void LoadModel(const std::filesystem::path& path);
		void Render(GLRenderer&);

		Camera& GetCamera() { return m_camera; } 
		PhysicsWorld& GetPhysicsWorld() { return m_physicsWorld; }
		const ModelContainer& GetModels() const { return m_models; }

	private:
		ModelContainer m_models;
		Camera m_camera;
		PhysicsWorld m_physicsWorld;
	};
}

