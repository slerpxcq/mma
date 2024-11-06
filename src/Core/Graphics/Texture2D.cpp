#include "CorePch.hpp"
#include "Texture2D.hpp"

#include "Graphics.hpp"

namespace mm
{

Texture2D::Texture2D(u32 width, u32 height, Graphics::TexFormat format) :
	Texture{ Target::TEXTURE_2D },
	m_format{ format },
	m_width{ width },
	m_height{ height }
{
	auto gfx = GetGraphics();
	gfx->TextureStorage2D(*this, width, height, format);
	/* BEGIN TEST CODE */
	gfx->SetTextureWrap2D(*this, Graphics::TexWrap::REPEAT, Graphics::TexWrap::REPEAT);
	gfx->SetTextureFilter(*this, Graphics::TexFilter::LINEAR, Graphics::TexFilter::LINEAR);
	/* END TEST CODE */
}

void Texture2D::SetSubImage(const void* data, 
							Graphics::PixelType type,
							Graphics::PixelFormat format,
							u32 width, u32 height,
							u32 level,
							u32 xoffset, u32 yoffset) const
{
	auto gfx = GetGraphics();
	gfx->TextureSubImage2D(*this, data, type, width, height, format, level, xoffset, yoffset);
}

void Texture2D::Resize(u32 width, u32 height)
{
	auto gfx = GetGraphics();
	gfx->DeleteTexture(*this);
	gfx->CreateTexture(*this);
	gfx->TextureStorage2D(*this, width, height, m_format);
}

}

