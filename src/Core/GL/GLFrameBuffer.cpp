#include "mmpch.hpp"
#include "GLFrameBuffer.hpp"

namespace mm
{
	GLFrameBuffer::GLFrameBuffer(glm::uvec2 size) :
		m_size(size)
	{
		glCreateFramebuffers(1, &m_id);
		//MM_ASSERT(sample >= 1);
		//if (sample == 1) {
		//	AddAttachment("Color0", GL_TEXTURE_2D, GL_COLOR_ATTACHMENT0, GL_RGBA8);
		//}
		//else {
		//	AddAttachment("Color0MS", GL_TEXTURE_2D_MULTISAMPLE, GL_COLOR_ATTACHMENT0, GL_RGBA8);
		//}
		//AddAttachment("Depth", GL_TEXTURE_2D, GL_DEPTH_ATTACHMENT, GL_DEPTH24_STENCIL8);
		Reload(m_size);
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_id);
		//glDeleteTextures(1, &m_colorTarget);
		//glDeleteTextures(1, &m_depthTarget);
	}

	//void GLFrameBuffer::AddAttachment(const std::string& name, uint32_t type, uint32_t target, uint32_t format)
	//{
	//	Attachment a = {};
	//	a.type = type;
	//	a.target = target;
	//	a.format = format;
	//	m_attachments.insert({ name, std::move(a) });
	//}

	void GLFrameBuffer::Reload(glm::uvec2 size)
	{
		if (m_id > 0) {
			//m_attachments.clear();
		//	glDeleteFramebuffers(1, &m_id);
			m_colorTarget.reset();
			m_depthTarget.reset();
			//glDeleteTextures(1, &m_colorTarget);
			//glDeleteTextures(1, &m_depthTarget);
		}

		m_size = size;

		//Attachment color0 = {};
		//color0.target = GL_COLOR_ATTACHMENT0;
		//color0.texture = std::make_unique<GLTexture2D>(m_size, GL_RGBA8);

		//Attachment depth = {};
		//depth.target = GL_DEPTH_ATTACHMENT;
		//depth.texture = std::make_unique<GLTexture2D>(m_size, GL_DEPTH24_STENCIL8);

		//m_attachments.insert({ "Color0", std::move(color0) });
		//m_attachments.insert({ "Depth", std::move(depth) });

		//for (auto& [_ ,a] : m_attachments) {
		//	a.texture.reset();
		//	switch (a.type) {
		//	case GL_TEXTURE_2D:
		//		a.texture = std::make_unique<GLTexture2D>(m_size, a.format);
		//		break;
		//	case GL_TEXTURE_2D_MULTISAMPLE:
		//		a.texture = std::make_unique<GLTexture2DMS>(m_size, a.format);
		//		break;
		//	}
		//	glNamedFramebufferTexture(m_id, a.target, a.texture->GetId(), 0);
		//}

		//m_attachments.insert({ "Color0", std::make_unique<GLTexture2D>(m_size, GL_RGBA8) });
		//m_attachments.insert({ "Depth", std::make_unique<GLTexture2D>(m_size, GL_DEPTH24_STENCIL8) });

		m_colorTarget = std::make_unique<GLTexture2D>(m_size, GL_RGBA8);
		m_depthTarget = std::make_unique<GLTexture2D>(m_size, GL_DEPTH24_STENCIL8);

		//glCreateFramebuffers(1, &m_id);
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_colorTarget);
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_depthTarget);
		//glTextureStorage2D(m_colorTarget, 1, GL_RGBA8, size.x, size.y);
		//glTextureStorage2D(m_depthTarget, 1, GL_DEPTH24_STENCIL8, size.x, size.y);
		//glTextureParameteri(m_colorTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTextureParameteri(m_colorTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTextureParameteri(m_depthTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTextureParameteri(m_depthTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0, m_colorTarget->GetId(), 0);
		//glNamedFramebufferTexture(m_id, GL_DEPTH_ATTACHMENT, m_depthTarget->GetId(), 0);
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
