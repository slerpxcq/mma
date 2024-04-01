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

		for (auto&& [_, model] : m_models) 
			model->Update(deltaTime);

		m_physicsWorld.Update(deltaTime);

		for (auto&& [_, model] : m_models) {
			model->SyncWithPhysics();
			model->CalcSkinning();
		}
	}

	void World::LoadModel(const std::filesystem::path& path)
	{
		try {
			auto model = std::make_unique<Model>(*this, path);
			m_models.insert({ model->GetPMXFile().GetInfo().nameJP, std::move(model) });
		}
		catch (const PMXParseError& e) {
			MM_FATAL("{0}: invalid file", path.u8string().c_str());
			return;
		}

		MM_INFO("{0}: model loaded", path.u8string().c_str());
	}

	void World::Render(GLRenderer& renderer) 
	{
		renderer.SetCamera(m_camera);

		for (auto&& [_, model] : m_models) {
			model->Render(renderer);
		}
	}
}
