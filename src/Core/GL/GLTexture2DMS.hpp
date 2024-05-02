#pragma once

#include "GLTexture.hpp"

namespace mm
{
	class GLTexture2DMS : public GLTexture
	{
	public:
		GLTexture2DMS(glm::uvec2 size, uint32_t format, uint32_t sampleCount) :
			GLTexture(GL_TEXTURE_2D_MULTISAMPLE),
			m_size(size), m_sampleCount(sampleCount) {
			glTextureStorage2DMultisample(m_id, m_sampleCount, format, m_size.x, m_size.y, GL_TRUE);
		}

	private:
		glm::uvec2 m_size;
		uint32_t m_sampleCount;
	};
}

