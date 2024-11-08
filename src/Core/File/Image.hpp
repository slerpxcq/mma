#pragma once

#include "File.hpp"
#include "../RectObject.hpp"

namespace mm
{

class Image : public File, public RectObject
{
public:
	explicit Image(const Path& path);
	Byte* GetPixels() const { return m_pixels.get(); }
	u32 GetWidth() const { return m_width; }
	u32 GetHeight() const { return m_height; }
	u32 GetChannels() const { return m_channels; }


private:
	u32 m_channels{};
	Scoped<Byte, Func<void(Byte*)>> m_pixels{};
};

}

