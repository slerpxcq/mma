#include "mmpch.hpp"
#include "GLFrameBuffer.hpp"

namespace mm
{
	GLFrameBuffer::GLFrameBuffer(glm::uvec2 size) :
		m_size(size)
	{
		glCreateFramebuffers(1, &m_id);
		Reload(m_size);
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_id);
	}

	void GLFrameBuffer::Reload(glm::uvec2 size)
	{
		if (m_id > 0) {
			m_colorTarget.reset();
			m_depthTarget.reset();
		}

		m_size = size;

		m_colorTarget = std::make_unique<GLTexture2D>(m_size, GL_RGBA8);
		m_depthTarget = std::make_unique<GLTexture2D>(m_size, GL_DEPTH24_STENCIL8);

		glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0, m_colorTarget->GetId(), 0);
		glNamedFramebufferTexture(m_id, GL_DEPTH_ATTACHMENT, m_depthTarget->GetId(), 0);

		MM_ASSERT(glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		MM_INFO("id={0}: framebuffer (re)loaded; size={1}x{2}", m_id, m_size.x, m_size.y);
	}

	void GLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}
}
