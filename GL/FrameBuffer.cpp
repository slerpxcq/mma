#include "RendererPch.hpp"
#include "FrameBuffer.hpp"

namespace mm
{

FrameBuffer::FrameBuffer()
{
	glCreateFramebuffers(1, &m_rendererID);
	MM_INFO("Framebuffer loaded successfully; id=%u", m_rendererID);
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept :
	GPUResource(std::move(other))
{
	m_size = other.m_size;
	m_depthAttachment = std::move(other.m_depthAttachment);
	m_colorAttachments = std::move(other.m_colorAttachments);
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
}

void FrameBuffer::Resize(glm::uvec2 size)
{
	//glDeleteFramebuffers(1, &m_rendererID);

	if (m_depthAttachment.Get()) {
		m_depthAttachment.Get()->Resize(size);
	}

	for (auto& attachment : m_colorAttachments) {
		attachment.Get()->Resize(size);
	}

	//glCreateFramebuffers(1, &m_rendererID);

	/* Re-attach all attachments */
	if (m_depthAttachment.Get()) {
		glNamedFramebufferTexture(m_rendererID, GL_DEPTH_ATTACHMENT, m_depthAttachment.Get()->GetID(), 0);
	}

	for (uint32_t i = 0; i < m_colorAttachments.size(); ++i) {
		glNamedFramebufferTexture(m_rendererID, GL_COLOR_ATTACHMENT0 + i, m_colorAttachments[i].Get()->GetID(), 0);
	}

	uint32_t result = glCheckNamedFramebufferStatus(m_rendererID, GL_FRAMEBUFFER);
	MM_ASSERT(result == GL_FRAMEBUFFER_COMPLETE);

	m_size = size;

	MM_INFO("Framebuffer resized successfully; id=%u", m_rendererID);
}

FrameBuffer::~FrameBuffer()
{
	if (m_rendererID > 0) {
		MM_INFO("Framebuffer unloaded successfully; id=%u", m_rendererID);
		glDeleteFramebuffers(1, &m_rendererID);
	}
}

}
