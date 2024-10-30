#include "CorePch.hpp"
#include "Graphics_GL.hpp"

#include "../Buffer.hpp"
#include "../VertexArray.hpp"
#include "../Texture.hpp"
#include "../FrameBuffer.hpp"

#include <glad/glad.h>

namespace mm
{

static GLuint ToGLBufferTarget(Buffer::Target target)
{
	switch (target) {
	case Buffer::Target::VERTEX:
		return GL_ARRAY_BUFFER;
	case Buffer::Target::INDEX:
		return GL_ELEMENT_ARRAY_BUFFER;
	default:
		MM_UNINPLEMENTED();
	}
}

static GLenum ToGLTexTarget(Texture::Target target)
{
	switch (target) {
	case Texture::Target::TEX_2D:
		return GL_TEXTURE_2D;
		break;
	default:
		MM_UNINPLEMENTED();
	}
}

static GLenum ToGLTexFormat(Graphics::TexFormat format)
{
	switch (format) {
	case Graphics::TexFormat::RGB8:
		return GL_RGB8;
		break;
	case Graphics::TexFormat::RGBA8:
		return GL_RGBA8;
		break;
	}
}

static GLenum ToGLPixelType(Graphics::PixelType type)
{
	switch (type) {
	case Graphics::PixelType::UBYTE:
		return GL_UNSIGNED_BYTE;
		break;
	}
}

static GLenum ToGLAttachment(Graphics::Attachment attachment, u32 index)
{
	switch (attachment) {
	case Graphics::Attachment::DEPTH:
		return GL_DEPTH_ATTACHMENT;
		break;
	case Graphics::Attachment::COLOR:
		return GL_COLOR_ATTACHMENT0 + index;
		break;
	}
}

void Graphics_GL::CreateBuffer(Buffer& buffer) const
{
	glCreateBuffers(1, buffer.GetIDPtr());
	MM_CORE_INFO("GL: buffer created; id={0}", buffer.GetID());
}

void Graphics_GL::DeleteBuffer(Buffer& buffer) const
{
	glDeleteBuffers(1, buffer.GetIDPtr());
	MM_CORE_INFO("GL: buffer deleted; id={0}", buffer.GetID());
}

void Graphics_GL::SetBufferData(const Buffer& buffer, const void* data, u32 size) const
{
	glNamedBufferData(buffer.GetID(), size, data, GL_STATIC_DRAW);
}

void Graphics_GL::SetBufferSubData(const Buffer& buffer, const void* data, u32 size, u32 offset) const
{
	glNamedBufferSubData(buffer.GetID(), offset, size, data);
}

void Graphics_GL::SetBufferBindBase(const Buffer& buffer, u32 base) const
{
	glBindBufferBase(ToGLBufferTarget(buffer.GetTarget()), base, buffer.GetID());
}

void Graphics_GL::CreateVertexArray(VertexArray& va) const
{
	glCreateVertexArrays(1, va.GetIDPtr());
	MM_CORE_INFO("GL: vertex array created; id={0}", va.GetID());
}

void Graphics_GL::DeleteVertexArray(VertexArray& va) const
{
	glDeleteVertexArrays(1, va.GetIDPtr());
	MM_CORE_INFO("GL: vertex array deleted; id={0}", va.GetID());
}

void Graphics_GL::SetIndexBuffer(const VertexArray& va, const IndexBuffer& ib) const
{
	glVertexArrayElementBuffer(va.GetID(), ib.GetID());
}

void Graphics_GL::SetVertexBuffer(const VertexArray& va, const VertexBuffer& vb) const
{
	// TODO: offset, binding point
	glVertexArrayVertexBuffer(va.GetID(), 0, vb.GetID(), 0, vb.GetLayout().GetSize());
}

void Graphics_GL::SetVertexAttribFormat(const VertexArray& va, u32 location, AttribType type, u32 count, u32 offset, bool normalized) const
{
	glEnableVertexArrayAttrib(va.GetID(), location);

	switch (type) {
	case AttribType::INT:
		glVertexArrayAttribIFormat(va.GetID(), location, count, GL_INT, offset);
		break;
	case AttribType::FLOAT:
		glVertexArrayAttribFormat(va.GetID(), location, count, GL_FLOAT, normalized, offset);
		break;
	}

	glVertexArrayAttribBinding(va.GetID(), location, 0);
}

void Graphics_GL::CreateTexture(Texture& tex) const
{
	glCreateTextures(ToGLTexTarget(tex.GetTarget()), 1, tex.GetIDPtr());
	MM_CORE_INFO("GL: texture created; id={0}", tex.GetID());
}

void Graphics_GL::DeleteTexture(Texture& tex) const
{
	glDeleteTextures(1, tex.GetIDPtr());
	MM_CORE_INFO("GL: texture deleted; id={0}", tex.GetID());
}

void Graphics_GL::BindTexture(const Texture& tex, u32 unit) const
{
	glBindTextureUnit(unit, tex.GetID());
}

void Graphics_GL::TextureStorage2D(const Texture& tex, u32 width, u32 height, TexFormat format, u32 levels) const
{
	glTextureStorage2D(tex.GetID(), levels, ToGLTexFormat(format), width, height);
}

void Graphics_GL::TextureSubImage2D(const Texture& tex, 
									const void* data, PixelType type, 
									u32 width, u32 height, 
									TexFormat format, u32 level, 
									u32 xoffset, u32 yoffset) const
{
	glTextureSubImage2D(tex.GetID(), level, xoffset, yoffset, width, height, ToGLTexFormat(format), ToGLPixelType(type), data);
}

void Graphics_GL::CreateFrameBuffer(FrameBuffer& fb) const
{
	glCreateFramebuffers(1, fb.GetIDPtr());
	MM_CORE_INFO("GL: frame buffer created; id={0}", fb.GetID());
}

void Graphics_GL::DeleteFrameBuffer(FrameBuffer& fb) const
{
	glDeleteFramebuffers(1, fb.GetIDPtr());
	MM_CORE_INFO("GL: frame buffer deleted; id={0}", fb.GetID());
}

void Graphics_GL::FrameBufferTexture(const FrameBuffer& fb, const Texture& tex, Attachment attachment, u32 index, u32 level) const
{
	glNamedFramebufferTexture(fb.GetID(), ToGLAttachment(attachment, index), tex.GetID(), level);
}

Graphics::FrameBufferStatus Graphics_GL::CheckFrameBufferStatus(const FrameBuffer& fb) const
{
	switch (glCheckNamedFramebufferStatus(fb.GetID(), GL_FRAMEBUFFER)) {
	case GL_FRAMEBUFFER_COMPLETE:
		return Graphics::FrameBufferStatus::OK;
		break;
	default:
		return Graphics::FrameBufferStatus::INCOMPLETE;
		break;
	}
}

}
