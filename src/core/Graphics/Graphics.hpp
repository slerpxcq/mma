#pragma once

namespace mm
{

class Buffer;
class VertexArray;
class IndexBuffer;
class VertexBuffer;
class Texture;

class Graphics
{
public:
	enum class AttribType { FLOAT, INT };
	enum class TexFormat { RGBA8, RGB8 };
	enum class PixelType { UBYTE };

public:
	virtual	~Graphics() = default;
	Graphics(Config::API api);
	Config::API GetAPI() const { return m_api; }

	virtual void CreateBuffer(Buffer&) const = 0;
	virtual void DeleteBuffer(Buffer&) const = 0;
	virtual void SetBufferData(const Buffer&, const void* data, u32 size) const = 0;
	virtual void SetBufferSubData(const Buffer&, const void* data, u32 size, u32 offset) const = 0;
	virtual void SetBufferBindBase(const Buffer&, u32 base) const = 0;

	virtual void CreateVertexArray(VertexArray&) const = 0;
	virtual void DeleteVertexArray(VertexArray&) const = 0;
	virtual void SetIndexBuffer(const VertexArray&, const IndexBuffer&) const = 0;
	virtual void SetVertexBuffer(const VertexArray&, const VertexBuffer&) const = 0;
	virtual void SetVertexAttribFormat(const VertexArray&, u32 location, AttribType type, 
									   u32 count, u32 offset, bool normalized = false) const = 0;

	virtual void CreateTexture(Texture&) const = 0;
	virtual void DeleteTexture(Texture&) const = 0;
	virtual void BindTexture(const Texture&, u32 unit) const = 0;
	virtual void TextureStorage2D(const Texture&, 
								  u32 width, u32 height, 
								  TexFormat format, u32 levels = 1) const = 0;
	virtual void TextureSubImage2D(const Texture&,
								   const void* data, PixelType type, 
								   u32 width, u32 height,
								   TexFormat format, u32 level = 0,
								   u32 xoffset = 0, u32 yoffset = 0) const = 0;

private:
	Config::API m_api{};
};

}

