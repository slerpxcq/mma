#include "CorePch.hpp"
#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace mm
{

Ref<Image> Image::Load(const Path& path)
{
	i32 x{}, y{}, ch{};
	Byte* pixels = reinterpret_cast<Byte*>(stbi_load(path.string().c_str(), &x, &y, &ch, 4));

	if (!pixels) {
		throw ImageLoadError(("Failed to load image; path=" + path.string()).c_str());
	}

	auto image = Ref<Image>(new Image{ path });
	image->m_width = x;
	image->m_height = y;
	image->m_channels = ch;
	image->m_pixels = 
		Scoped<Byte, Func<void(Byte*)>>(pixels, 
											[](Byte* ptr) {
												stbi_image_free(ptr); 
											});

	return image;
}

}

