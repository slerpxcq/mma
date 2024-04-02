#include "mmpch.hpp"
#include "GLFrameBuffer.hpp"

namespace mm
{
	GLFrameBuffer::GLFrameBuffer(glm::uvec2 size) :
		m_size(size)
	{
		Reload(m_size);
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_id);
		glDeleteTextures(1, &m_colorTarget);
		glDeleteTextures(1, &m_depthTarget);
	}

	void GLFrameBuffer::Reload(glm::uvec2 size)
	{
		if (m_id > 0) {
			glDeleteFramebuffers(1, &m_id);
			glDeleteTextures(1, &m_colorTarget);
			glDeleteTextures(1, &m_depthTarget);
		}

		m_size = size;

		glCreateFramebuffers(1, &m_id);
		glCreateTextures(GL_TEXTURE_2D, 1, &m_colorTarget);
		glCreateTextures(GL_TEXTURE_2D, 1, &m_depthTarget);
		glTextureStorage2D(m_colorTarget, 1, GL_RGBA8, size.x, size.y);
		glTextureStorage2D(m_depthTarget, 1, GL_DEPTH24_STENCIL8, size.x, size.y);
		//glTextureParameteri(m_colorTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTextureParameteri(m_colorTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTextureParameteri(m_depthTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTextureParameteri(m_depthTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0, m_colorTarget, 0);
		glNamedFramebufferTexture(m_id, GL_DEPTH_ATTACHMENT, m_depthTarget, 0); 

		MM_ASSERT(glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		MM_INFO("id={0}: framebuffer (re)loaded; size={1}x{2}", m_id, m_size.x, m_size.y);
	}

	void GLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}
}
