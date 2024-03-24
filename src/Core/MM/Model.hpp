#pragma once

#include "PMXFile.hpp"
#include "Armature.hpp"
#include "Skin.hpp"

namespace mm
{
	class GLRenderer;
	class Model
	{
		friend class Armature;
		friend class Skin;

	public:
		Model(const std::filesystem::path& path);
		void Render(GLRenderer* renderer);

	private:
		std::unique_ptr<PMXFile> m_pmxFile;

		Armature m_armature;
		Skin m_skin;
	};
}

