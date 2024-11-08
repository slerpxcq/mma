#include "CorePch.hpp"
#include "Texture2DArray.hpp"

namespace mm
{

Texture2DArray::Texture2DArray(u32 width, u32 height, u32 depth, Graphics::TexFormat format) :
	Texture{ Target::TEXTURE_2D_ARRAY },
	CubicObject{ width, height, depth }
{
	auto gfx = GetGraphics();
	gfx->TextureStorage3D(*this, width, height, depth, format);
}

void Texture2DArray::SetSubImage(const void* data, 
								 Graphics::PixelType type, 
								 Graphics::PixelFormat format, 
								 u32 width, u32 height, u32 zoffset, 
								 u32 level, u32 xoffset, u32 yoffset) const
{
	auto gfx = GetGraphics();
	gfx->TextureSubImage3D(*this, data, type,
						   width, height, 1,
						   format, level, 
						   xoffset, yoffset, zoffset);
}

}
