#include "mmpch.hpp"
#include "World.hpp"

#include "Core/GL/GLRenderer.hpp"
#include "Model/Armature.hpp"

namespace mm
{
	World::World()
	{
	}

	void World::OnUpdate(float deltaTime)
	{
		for (auto& model : m_models) 
			model->Update(deltaTime);

		m_physicsWorld.Update(deltaTime);

		for (auto& model : m_models) {
			model->SyncWithPhysics();
			model->CalcSkinning();
		}
	}

	Model* World::LoadModel(const std::filesystem::path& path)
	{
		Model* ret = nullptr;
		try {
			auto model = std::make_unique<Model>(*this, path);
			ret = model.get();
			m_models.push_back(std::move(model));
		}
		catch (const PMXParseError& e) {
			MM_FATAL("{0}: invalid file", path.u8string().c_str());
			return nullptr;
		}

		MM_INFO("{0}: model loaded", path.u8string().c_str());
		return ret;
	}

	void World::Render(GLRenderer& renderer) 
	{
		for (auto& model : m_models) {
			model->Render(renderer);
		}
	}
}
