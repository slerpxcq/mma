#include "RendererPch.hpp"
#include "Texture2D.hpp"

namespace mm
{

Texture2D::Texture2D(glm::uvec2 size, uint32_t format, bool createHandle) :
	m_size(size), m_format(format),
	Texture(GL_TEXTURE_2D)
{
	glTextureStorage2D(m_rendererID, 1, format, size.x, size.y);
}

Texture2D::Texture2D(Texture2D&& other) noexcept :
	Texture(std::move(other))
{
	m_size = other.m_size;
	m_format = other.m_format;
}

void Texture2D::SubImage(uint32_t level,
						 uint32_t xoffset,
						 uint32_t yoffset,
						 uint32_t width,
						 uint32_t height,
						 uint32_t format, 
						 uint32_t type,
						 const void* pixels)
{
	glTextureSubImage2D(m_rendererID,
						level,
						xoffset,
						yoffset,
						width,
						height,
						format,
						type,
						pixels);
}

void Texture2D::Resize(glm::uvec2 size)
{
	glDeleteTextures(1, &m_rendererID);
	glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
	glTextureStorage2D(m_rendererID, 1, m_format, size.x, size.y);
	m_size = size;
}

}
