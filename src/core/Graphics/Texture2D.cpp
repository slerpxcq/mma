#include "CorePch.hpp"
#include "Texture2D.hpp"

namespace mm
{

Texture2D::Texture2D(u32 width, u32 height, Graphics::TexFormat format) :
	Texture{ Target::TEX_2D },
	m_format{ format },
	m_width{ width },
	m_height{ height }
{
	auto gfx = GetGraphics();
	gfx->TextureStorage2D(*this, width, height, format);
}

void Texture2D::SetSubImage(const void* data, Graphics::PixelType type,
							u32 width, u32 height,
							u32 level,
							u32 xoffset, u32 yoffset) const
{
	auto gfx = GetGraphics();
	gfx->TextureSubImage2D(*this, data, type, width, height, m_format, level, xoffset, yoffset);
}

void Texture2D::Resize(u32 width, u32 height)
{
	auto gfx = GetGraphics();
	gfx->DeleteTexture(*this);
	gfx->CreateTexture(*this);
	gfx->TextureStorage2D(*this, width, height, m_format);
}

}

