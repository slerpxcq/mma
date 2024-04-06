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

		uint32_t GetColorTarget() const { return m_colorTarget; }
		glm::uvec2 GetSize() const { return m_size;  }
		void BindColorTarget(uint32_t unit) const { glBindTextureUnit(unit, m_colorTarget); }

	private:
		uint32_t m_id = 0;
		uint32_t m_colorTarget = 0;
		uint32_t m_depthTarget = 0;
		glm::uvec2 m_size;
	};
}

