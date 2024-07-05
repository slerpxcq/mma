#include "RendererPch.hpp"
#include "Texture.hpp"

#include <glad/gl.h>

namespace mm
{

Texture::Texture(uint32_t target, bool createHandle) :
	m_target(target)
{
	glCreateTextures(target, 1, &m_rendererID);
	MM_INFO("Texture loaded successfully; id=%u", m_rendererID, m_handle);
}

Texture::Texture(Texture&& other) noexcept :
	GPUResource(std::move(other))
{
	m_target = other.m_target;
}

Texture::~Texture()
{
	if (m_rendererID > 0) {
		MM_INFO("Texture unloaded successfully; id=%u", m_rendererID);
		glDeleteTextures(1, &m_rendererID);
	}
}

void Texture::Bind(uint32_t unit) const
{
	glBindTextureUnit(unit, m_rendererID);
}

void Texture::CreateHandle()
{
	m_handle = glGetTextureHandleARB(m_rendererID);
	MM_ASSERT(m_handle && "Failed to create texture handle; id=%u", m_rendererID);
	glMakeTextureHandleResidentARB(m_handle);
}

}

