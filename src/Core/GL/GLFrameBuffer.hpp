#pragma once

#include "GLTexture.hpp"

namespace mm
{
	class GLFrameBuffer
	{
	public:
		GLFrameBuffer(glm::uvec2 size);
		~GLFrameBuffer();

		void Reload(glm::uvec2 size);

		void Bind();
		void Unbind();

	private:
		uint32_t m_id = 0;
		glm::uvec2 m_size;
		uint32_t m_colorTarget = 0;
		uint32_t m_depthTarget = 0;
	};
}

