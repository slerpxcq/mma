#include "mmpch.hpp"
#include "GLFrameBufferMS.hpp"

namespace mm
{
	GLFrameBufferMS::GLFrameBufferMS(glm::uvec2 size) :
		m_size(size)
	{
		glCreateFramebuffers(1, &m_id);
	}

	GLFrameBufferMS::~GLFrameBufferMS()
	{
		glDeleteFramebuffers(1, &m_id);
	}

	void GLFrameBufferMS::Reload(glm::uvec2 size)
	{
		if (m_id > 0) {
			m_blitTarget.reset();
			m_colorTarget.reset();
			m_depthTarget.reset();
		}

		m_size = size;

		m_blitTarget = std::make_unique<GLFrameBuffer>(m_size);
		m_colorTarget = std::make_unique<GLTexture2DMS>(m_size, GL_RGBA8, 4);
		m_depthTarget = std::make_unique<GLTexture2DMS>(m_size, GL_DEPTH24_STENCIL8, 4);

		glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0, m_colorTarget->GetId(), 0);
		glNamedFramebufferTexture(m_id, GL_DEPTH_ATTACHMENT, m_depthTarget->GetId(), 0);

		int32_t status = glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER);
		MM_INFO("status={0}", status);
		MM_ASSERT(status == GL_FRAMEBUFFER_COMPLETE);
		MM_INFO("id={0}: multisample framebuffer (re)loaded; size={1}x{2}", m_id, m_size.x, m_size.y);
	}

	void GLFrameBufferMS::Blit()
	{
		glBlitNamedFramebuffer(
			m_id, m_blitTarget->GetId(),
			0, 0, m_size.x, m_size.y,
			0, 0, m_size.x, m_size.y,
			GL_COLOR_BUFFER_BIT,
			GL_NEAREST
		);
	}

	void GLFrameBufferMS::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}
}
