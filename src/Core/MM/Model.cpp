#include "mmpch.hpp"
#include "Model.hpp"

#include "Core/GL/GLRenderer.hpp"

namespace mm
{
	Model::Model(const std::filesystem::path& path) :
		m_pmxFile(std::make_unique<PMXFile>(path)),
		m_armature(*this),
		m_skin(*this)
	{
	}

	void Model::Render(GLRenderer* renderer)
	{
	}
}
