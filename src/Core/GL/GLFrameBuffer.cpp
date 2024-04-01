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

		glCreateTextures(GL_TEXTURE_2D, 1, &m_colorTarget);
		glCreateTextures(GL_TEXTURE_2D, 1, &m_depthTarget);
		glTextureStorage2D(m_colorTarget, 0, GL_RGBA8, size.x, size.y);
		glTextureStorage2D(m_depthTarget, 0, GL_DEPTH24_STENCIL8, size.x, size.y);
		//glTextureParameteri(m_colorTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTextureParameteri(m_colorTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTextureParameteri(m_depthTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTextureParameteri(m_depthTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(m_id, GL_COLOR_ATTACHMENT0, m_colorTarget, GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(m_id, GL_DEPTH_ATTACHMENT, m_depthTarget, GL_TEXTURE_2D, 0); 

		MM_ASSERT(glCheckFramebufferStatus(m_id) == GL_FRAMEBUFFER_COMPLETE);
		MM_INFO("id={0}: framebuffer (re)loaded");
	}

	void GLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}

	void GLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
