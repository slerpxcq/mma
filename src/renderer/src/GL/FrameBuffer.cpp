#include "RendererPch.hpp"
#include "FrameBuffer.hpp"

#include "../GPUResourceManager.hpp"

namespace mm
{

std::shared_ptr<FrameBuffer> FrameBuffer::CreateDefault(const std::string& name, glm::uvec2 size)
{
	auto fb = GPUResourceManager::Instance().Load<FrameBuffer>(name);
	fb->m_size = size;

	auto color = GPUResourceManager::Instance().Load<Texture2D>("tex_color0_" + name, size, GL_RGBA8);
	auto depth = GPUResourceManager::Instance().Load<Texture2D>("tex_depth_" + name, size, GL_DEPTH24_STENCIL8);

	glNamedFramebufferTexture(fb->GetID(), GL_COLOR_ATTACHMENT0, color->GetID(), 0);
	glNamedFramebufferTexture(fb->GetID(), GL_DEPTH_ATTACHMENT, depth->GetID(), 0);

	uint32_t result = glCheckNamedFramebufferStatus(fb->GetID(), GL_FRAMEBUFFER);
	MM_ASSERT(result == GL_FRAMEBUFFER_COMPLETE);

	fb->m_depthStencilAttachment = depth;
	fb->m_colorAttachments.push_back(color);

	return fb;
}

FrameBuffer::FrameBuffer()
{
	glCreateFramebuffers(1, &m_rendererID);
	MM_INFO("Framebuffer loaded successfully; id=%u", m_rendererID);
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept :
	GPUResource(std::move(other))
{
	m_size = other.m_size;
	m_depthStencilAttachment = std::move(other.m_depthStencilAttachment);
	m_colorAttachments = std::move(other.m_colorAttachments);
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
}

void FrameBuffer::Resize(glm::uvec2 size)
{
	if (m_depthStencilAttachment) {
		m_depthStencilAttachment->Resize(size);
	}

	for (auto& attachment : m_colorAttachments) {
		attachment->Resize(size);
	}

	if (m_depthStencilAttachment) {
		glNamedFramebufferTexture(m_rendererID, GL_DEPTH_ATTACHMENT, m_depthStencilAttachment->GetID(), 0);
	}

	for (uint32_t i = 0; i < m_colorAttachments.size(); ++i) {
		glNamedFramebufferTexture(m_rendererID, GL_COLOR_ATTACHMENT0 + i, m_colorAttachments[i]->GetID(), 0);
	}

	uint32_t result = glCheckNamedFramebufferStatus(m_rendererID, GL_FRAMEBUFFER);
	MM_ASSERT(result == GL_FRAMEBUFFER_COMPLETE);

	m_size = size;

	MM_INFO("Framebuffer resized successfully; id=%u", m_rendererID);
}

void FrameBuffer::Clear(glm::vec4 color, uint32_t bitmask) const
{
	int32_t fb;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);

	glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(bitmask);

	glBindFramebuffer(GL_FRAMEBUFFER, fb);
}

FrameBuffer::~FrameBuffer()
{
	if (m_rendererID > 0) {
		MM_INFO("Framebuffer unloaded successfully; id=%u", m_rendererID);
		glDeleteFramebuffers(1, &m_rendererID);
	}
}

}
