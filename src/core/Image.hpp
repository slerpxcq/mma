#pragma once

namespace mm
{

class Image
{
public:
	struct ImageLoadError : public RuntimeError {
		ImageLoadError(const char* what) : RuntimeError{ what } {}
	};

public:
	static Ref<Image> Load(const Path& path);
	Byte* GetPixels() const { return m_pixels.get(); }
	u32 GetWidth() const { return m_width; }
	u32 GetHeight() const { return m_height; }
	u32 GetChannels() const { return m_channels; }

public:
	Image() = default;

private:
	u32 m_width{};
	u32 m_height{};
	u32 m_channels{};
	Scoped<Byte, Function<void(Byte*)>> m_pixels{};
};

}

