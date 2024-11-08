#pragma once

#include "Texture.hpp"
#include "../RectObject.hpp"
#include "Graphics.hpp"

namespace mm
{

class Texture2D : public Texture, public RectObject
{
public:
	Texture2D(u32 width, u32 height, Graphics::TexFormat format);
	void SetSubImage(const void* data, 
					 Graphics::PixelType type,
					 Graphics::PixelFormat format,
					 u32 width, u32 height,
					 u32 level = 0,
					 u32 xoffset = 0, u32 yoffset = 0) const;
	Graphics::TexFormat GetFormat() const { return m_format; }
	void Resize(u32 width, u32 height);

private:
	Graphics::TexFormat m_format{};
};

}

