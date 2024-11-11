#pragma once

namespace mm
{

class Buffer;
class VertexArray;
class IndexBuffer;
class VertexBuffer;
class Texture;
class Texture2D;
class FrameBuffer;
class Shader;
class Program;

class Graphics
{
public:
	enum class AttribType { FLOAT, INT };
	enum class TexFormat { RGBA8, RGB8, D24S8 };
	enum class PixelType { UBYTE };
	enum class PixelFormat { RGB, RGBA };
	enum class AttachmentType { COLOR, DEPTH };
	enum class FrameBufferStatus { OK, INCOMPLETE };
	enum class ShaderType { VERTEX, FRAGMENT };
	enum class IndexType { UBYTE, USHORT, UINT };
	enum class FrontFace { CW, CCW };
	enum class TexWrap { REPEAT, CLAMP_TO_BORDER, CLAMP_TO_EDGE };
	enum class TexFilter { LINEAR, NEAREST };
	enum class BlendFactor { SRC_ALPHA, ONE_MINUS_SRC_ALPHA };
	enum class DrawMode { TRIANGLES, LINES };
	enum BufferFlags {
		NONE = 0,
		MAP_READ_BIT = 1<<0,
		MAP_WRITE_BIT = 1<<1,
		DYNAMIC_STORAGE_BIT = 1<<2
	};
	enum class BufferAccess {
		READ = 1<<0,
		WRITE = 1<<1,
		READ_WRITE = READ | WRITE
	};

public:
	static u32 ToSize(Graphics::IndexType type);
	static IndexType ToIndexType(u32 size);

public:
	virtual	~Graphics() = default;
	Graphics(Config::API api);
	Config::API GetAPI() const { return m_api; }

	virtual void SetViewport(Vec2 size, Vec2 pos = { 0, 0 }) const = 0;
	virtual void SetFrontFace(FrontFace) const = 0;
	virtual void SetCulling(bool enable) const = 0;
	virtual void SetDepthTest(bool enable) const = 0;
	virtual void SetBlend(bool enable) const = 0;
	virtual void SetBlendFunc(BlendFactor src, BlendFactor dst) const = 0;

	virtual void CreateBuffer(Buffer&) const = 0;
	virtual void DeleteBuffer(Buffer&) const = 0;
	virtual void SetBufferStorage(const Buffer&, const void* data, u32 size, BufferFlags flags = BufferFlags::NONE) const = 0;
	virtual void SetBufferSubData(const Buffer&, const void* data, u32 size, u32 offset) const = 0;
	virtual void SetBufferBindBase(const Buffer&, u32 base) const = 0;
	virtual void* MapBuffer(const Buffer&, BufferAccess access) const = 0;
	virtual void UnmapBuffer(const Buffer&) const = 0;

	virtual void CreateVertexArray(VertexArray&) const = 0;
	virtual void DeleteVertexArray(VertexArray&) const = 0;
	virtual void SetIndexBuffer(const VertexArray&, const IndexBuffer&) const = 0;
	virtual void SetVertexBuffer(const VertexArray&, const VertexBuffer&) const = 0;
	virtual void SetVertexAttribFormat(const VertexArray&, u32 location, AttribType type, 
									   u32 count, u32 offset, bool normalized = false) const = 0;
	virtual void DrawElements(const VertexArray& va, u32 begin, u32 count) const = 0;
	virtual void DrawArrays(DrawMode mode, u32 begin, u32 count) const = 0;

	virtual void CreateTexture(Texture&) const = 0;
	virtual void DeleteTexture(Texture&) const = 0;
	virtual void BindTexture(const Texture&, u32 unit) const = 0;
	virtual void TextureStorage2D(const Texture&, 
								  u32 width, u32 height, 
								  TexFormat format, u32 levels = 1) const = 0;
	virtual void TextureSubImage2D(const Texture&,
								   const void* data, PixelType type, 
								   u32 width, u32 height,
								   PixelFormat format, u32 level = 0,
								   u32 xoffset = 0, u32 yoffset = 0) const = 0;
	virtual void TextureStorage3D(const Texture&,
								  u32 width, u32 height, u32 depth,
								  TexFormat format, u32 levels = 1) const = 0;
	virtual void TextureSubImage3D(const Texture&,
								   const void* data, PixelType type,
								   u32 width, u32 height, u32 depth,
								   PixelFormat format, u32 level = 0,
								   u32 xoffset = 0, u32 yoffset = 0, u32 zoffset = 0) const = 0;
	virtual void SetTextureFilter(const Texture&, TexFilter min, TexFilter mag) const = 0;
	virtual void SetTextureWrap2D(const Texture2D&, TexWrap u, TexWrap v) const = 0;

	virtual void CreateFrameBuffer(FrameBuffer&) const = 0;
	virtual void DeleteFrameBuffer(FrameBuffer&) const = 0;
	virtual void FrameBufferTexture(const FrameBuffer&, const Texture&, 
									AttachmentType attachment, u32 index, 
									u32 level = 0) const = 0;
	virtual FrameBufferStatus CheckFrameBufferStatus(const FrameBuffer&) const = 0;
	virtual void ClearFrameBufferColor(const FrameBuffer&, u32 index = 0, 
									   Color color = { 0.f, 0.f, 0.f, 0.f }) const = 0;
	virtual void ClearFrameBufferDepth(const FrameBuffer&, 
									   f32 depth = 0.f, i32 stencil = 0) const = 0;
	virtual void BindFrameBuffer(const FrameBuffer&) = 0;
	virtual void UnbindFrameBuffer() = 0;
	virtual void BindWindowFrameBuffer() const = 0;

	virtual Opt<String> CreateShader(Shader&, StringView source, ShaderType type) const = 0;
	virtual void DeleteShader(Shader&) const = 0;

	virtual void CreateProgram(Program&) const = 0;
	virtual void DeleteProgram(Program&) const = 0;
	virtual void AttachShader(const Program&, const Shader&) const = 0;
	virtual Opt<String> LinkProgram(const Program&) const = 0;
	virtual void UseProgram(const Program&) const = 0;
	virtual u32 GetUniformCount(const Program&) const = 0;
	virtual String GetUniformName(const Program&, u32 index) const = 0;
	virtual i32 GetUniformLocation(const Program&, StringView name) const = 0;
	virtual void SetUniform(const Program&, i32 location, u32 count, const f32* val) const = 0;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec2* val) const = 0;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec3* val) const = 0;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec4* val) const = 0;
	virtual void SetUniform(const Program&, i32 location, u32 count, const i32* val) const = 0;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec2i* val) const = 0;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec3i* val) const = 0;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec4i* val) const = 0;
	virtual void SetUniform(const Program&, i32 location, u32 count, const u32* val) const = 0;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec2u* val) const = 0;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec3u* val) const = 0;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec4u* val) const = 0;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Mat4* val, bool transpose = false) const = 0;

private:
	Config::API m_api{};
};

}

