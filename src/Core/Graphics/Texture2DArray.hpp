#pragma once

#include "Texture.hpp"
#include "CubicObject.hpp"
#include "Graphics.hpp"

namespace mm
{

class Texture2DArray : public Texture, public CubicObject
{
public:
	Texture2DArray(u32 width, u32 height, u32 depth,
				   Graphics::TexFormat format);
	void SetSubImage(const void* data, 
					 Graphics::PixelType type,
					 Graphics::PixelFormat format,
					 u32 width, u32 height, 
					 u32 zoffset,
					 u32 level = 0,
					 u32 xoffset = 0, u32 yoffset = 0) const;

private:
};

}

