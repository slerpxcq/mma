#pragma once

#include "Texture.hpp"

namespace mm
{

class Texture2D : public Texture
{
public:
	Texture2D(u32 width, u32 height, Graphics::TexFormat format);
	void SetSubImage(const void* data, Graphics::PixelType type,
						u32 width, u32 height,
						u32 level = 0,
						u32 xoffset = 0, u32 yoffset = 0) const;
	u32 GetWidth() const { return m_width; }
	u32 GetHeight() const { return m_height; }

private:
	u32 m_width{};
	u32 m_height{};
	Graphics::TexFormat m_format{};
};

}

