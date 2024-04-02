#pragma once

#include "Core/Camera/Camera.hpp"
#include "Core/MM/Model/Model.hpp"
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
		Model* LoadModel(const std::filesystem::path& path);
		void Render(GLRenderer&);

		PhysicsWorld& GetPhysicsWorld() { return m_physicsWorld; }
		const ModelContainer& GetModels() const { return m_models; }

	private:
		ModelContainer m_models;
		PhysicsWorld m_physicsWorld;
	};
}

