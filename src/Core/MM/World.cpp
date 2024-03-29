#include "mmpch.hpp"
#include "World.hpp"

#include "Core/GL/GLRenderer.hpp"
#include "Armature.hpp"

namespace mm
{
	World::World()
	{
	}

	void World::OnUpdate(float deltaTime)
	{
		m_camera.Update();

		for (auto& model : m_models) 
			model->Update(deltaTime);

		m_physicsWorld.Update(deltaTime);

		for (auto& model : m_models) {
			model->SyncWithPhysics();
			model->CalcSkinning();
		}
	}

	void World::LoadModel(const std::filesystem::path& path)
	{
		try {
			m_models.push_back(std::make_unique<Model>(*this, path));
		}
		catch (const PMXParseError& e) {
			MM_FATAL("{0}: invalid file", path.u8string().c_str());
			return;
		}

		MM_INFO("{0}: model loaded", path.u8string().c_str());
	}

	void World::Render(GLRenderer& renderer) 
	{
		renderer.SetCamera(&m_camera);

		for (auto& model : m_models) {
			model->Render(renderer);
		}
	}
}
