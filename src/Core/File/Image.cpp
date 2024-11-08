#include "CorePch.hpp"
#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace mm
{

Image::Image(const Path& path) :
	File{path}
{
	i32 x{}, y{}, ch{};
	Byte* pixels = reinterpret_cast<Byte*>(stbi_load(path.string().c_str(), &x, &y, &ch, 4));

	if (!pixels) {
		throw FileLoadError(("Failed to load image; path=" + path.string()).c_str());
	}

	m_width = x;
	m_height = y;
	m_channels = ch;
	m_pixels = Scoped<Byte, Func<void(Byte*)>>(pixels, 
											   [](Byte* ptr) {
											       stbi_image_free(ptr); 
											   });
}

}

