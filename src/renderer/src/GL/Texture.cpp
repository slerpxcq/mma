#include "RendererPch.hpp"
#include "Texture.hpp"

namespace mm
{

Texture::Texture(uint32_t target) :
	m_target(target)
{
	glCreateTextures(target, 1, &m_rendererID);
	MM_INFO("Texture loaded successfully; id={0}", m_rendererID);
}

Texture::Texture(Texture&& other) noexcept :
	GPUResource(std::move(other))
{
	m_target = other.m_target;
}

Texture::~Texture()
{
	if (m_rendererID > 0) {
		MM_INFO("Texture unloaded successfully; id={0}", m_rendererID);
		glDeleteTextures(1, &m_rendererID);
	}
}

void Texture::Bind(uint32_t unit) const
{
	glBindTextureUnit(unit, m_rendererID);
}

}

