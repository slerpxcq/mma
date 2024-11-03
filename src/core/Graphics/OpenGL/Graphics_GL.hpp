#pragma once

#include "../Graphics.hpp"

namespace mm
{

class Graphics_GL : public Graphics
{
public:
	Graphics_GL() : Graphics{ Config::API::GL4 } {}

	virtual void CreateBuffer(Buffer&) const override;
	virtual void DeleteBuffer(Buffer&) const override;
	virtual void SetBufferData(const Buffer&, const void* data, u32 size) const override;
	virtual void SetBufferSubData(const Buffer&, const void* data, u32 size, u32 offset) const override;
	virtual void SetBufferBindBase(const Buffer&, u32 base) const override;

	virtual void CreateVertexArray(VertexArray&) const override;
	virtual void DeleteVertexArray(VertexArray&) const override;	
	virtual void SetIndexBuffer(const VertexArray&, const IndexBuffer&) const override;
	virtual void SetVertexBuffer(const VertexArray&, const VertexBuffer&) const override;
	virtual void SetVertexAttribFormat(const VertexArray&, u32 location, AttribType type, 
									   u32 count, u32 offset, bool normalized) const override;
	virtual void DrawElements(const VertexArray& va, u32 begin, u32 count) const override;

	virtual void CreateTexture(Texture&) const override;
	virtual void DeleteTexture(Texture&) const override;
	virtual void BindTexture(const Texture&, u32 unit) const override; 
	virtual void TextureStorage2D(const Texture&, 
								  u32 width, u32 height, 
								  TexFormat format, u32 levels) const override;
	virtual void TextureSubImage2D(const Texture&,
								   const void* data, PixelType type, 
								   u32 width, u32 height,
								   TexFormat format, u32 level,
								   u32 xoffset, u32 yoffset) const override;

	virtual void CreateFrameBuffer(FrameBuffer&) const override;
	virtual void DeleteFrameBuffer(FrameBuffer&) const override;
	virtual void FrameBufferTexture(const FrameBuffer&, const Texture&, 
									AttachmentType attachment, u32 index, u32 level) const override;
	virtual FrameBufferStatus CheckFrameBufferStatus(const FrameBuffer&) const override;
	virtual void ClearFrameBufferColor(const FrameBuffer&, u32 index, 
									   Color color) const override;
	virtual void ClearFrameBufferDepth(const FrameBuffer&, 
									   f32 depth, i32 stencil) const override;
	virtual void BindFrameBuffer(const FrameBuffer&) override;
	virtual void UnbindFrameBuffer() override;
	virtual void BindWindowFrameBuffer() const override;

	virtual Opt<String> CreateShader(Shader&, StringView source, ShaderType type) const override;
	virtual void DeleteShader(Shader&) const override;
	virtual void AttachShader(const Program&, const Shader&) const override;

	virtual void CreateProgram(Program&) const override;
	virtual void DeleteProgram(Program&) const override;
	virtual Opt<String> LinkProgram(const Program&) const override;
	virtual void UseProgram(const Program&) const override;
	virtual u32 GetUniformCount(const Program&) const override;
	virtual String GetUniformName(const Program&, u32 index) const override;
	virtual i32 GetUniformLocation(const Program&, StringView name) const override;
	virtual void SetUniform(const Program&, i32 location, u32 count, const f32* val) const override;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec2* val) const override;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec3* val) const override;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec4* val) const override;
	virtual void SetUniform(const Program&, i32 location, u32 count, const i32* val) const override;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec2i* val) const override;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec3i* val) const override;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec4i* val) const override;
	virtual void SetUniform(const Program&, i32 location, u32 count, const u32* val) const override;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec2u* val) const override;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec3u* val) const override;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Vec4u* val) const override;
	virtual void SetUniform(const Program&, i32 location, u32 count, const Mat4* val, bool transpose = false) const override;

private:
	i32 m_lastFramebuffer{};
};

}

