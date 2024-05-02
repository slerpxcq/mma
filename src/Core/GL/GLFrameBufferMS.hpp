#pragma once

#include "GLTexture2D.hpp"
#include "GLTexture2DMS.hpp"
#include "GLFrameBuffer.hpp"

namespace mm
{
	class GLFrameBufferMS
	{
	public:
		GLFrameBufferMS(glm::uvec2 size);
		void Reload(glm::uvec2 size);

		void Blit();
		void Bind();

		glm::uvec2 GetSize() { return m_size; }

		uint32_t GetColorTarget() { return m_blitTarget->GetColorTarget(); }
		void BindColorTarget(uint32_t unit) { m_blitTarget->BindColorTarget(unit); }
		~GLFrameBufferMS();

	private:
		uint32_t m_id = 0;

		std::unique_ptr<GLFrameBuffer> m_blitTarget;

		std::unique_ptr<GLTexture2DMS> m_colorTarget;
		std::unique_ptr<GLTexture2DMS> m_depthTarget;

		glm::uvec2 m_size;
	};
}

