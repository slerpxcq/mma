#include "mmpch.hpp"
#include "World.hpp"

#include "Core/MM/Renderer/Renderer.hpp"
#include "Core/MM/Model/Armature.hpp"
#include "Core/MM/Lights/DirectionalLight.hpp"

namespace mm
{
	World::World()
	{
		m_directionalLight.SetColor(glm::vec3(.6, .6, .6));
		m_directionalLight.SetDirection(glm::vec3(-.5, -1, .5));
	}

	World::~World()
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

	void World::Render(Renderer& renderer) 
	{
		LightLayout l = {};
		l.color = glm::vec4(m_directionalLight.GetColor(), 0);
		l.direction = glm::vec4(m_directionalLight.GetDirection(), 0);
		renderer.SetLight(l);

		for (auto& model : m_models) {
			model->Render(renderer);
		}
	}
}
