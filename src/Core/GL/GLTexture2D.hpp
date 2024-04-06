#pragma once

#include "GLTexture.hpp"

namespace mm
{
	class GLTexture2D : public GLTexture
	{
	public:
		GLTexture2D(const std::filesystem::path&);
		//GLTexture2D(glm::uvec2 size);

	private:
		glm::uvec2 m_size;
	};
}

