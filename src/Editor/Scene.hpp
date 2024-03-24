#pragma once

#include "Core/MM/Model.hpp"

namespace mm
{
	class GLRenderer;
	class Scene
	{
	public:
		void LoadModel(const std::filesystem::path& path);
		void Render(GLRenderer*);

	private:
		// TODO: model render order
		std::vector<Model> m_models;
	};
}

