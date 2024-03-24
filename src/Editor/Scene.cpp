#include "mmpch.hpp"
#include "Scene.hpp"

#include "Core/GL/GLRenderer.hpp"

namespace mm
{
	void Scene::LoadModel(const std::filesystem::path& path)
	{
		try {
			m_models.emplace_back(path);
		}
		catch (const PMXParseError& e) {
			MM_FATAL("{0}: Invalid model file", path.u8string().c_str());
			return;
		}

		MM_INFO("{0}: Model successfully loaded", path.u8string().c_str());
	}

	void Scene::Render(GLRenderer* renderer) 
	{
		for (auto& model : m_models) {
			model.Render(renderer);
		}
	}
}
