#include "CorePch.hpp"
#include "Graphics_GL.hpp"

#include "../Buffer.hpp"
#include "../VertexArray.hpp"
#include "../Texture.hpp"
#include "../FrameBuffer.hpp"
#include "../Shader.hpp"
#include "../Program.hpp"

#include <glm/gtc/type_ptr.hpp>
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
	case Buffer::Target::UNIFORM:
		return GL_UNIFORM_BUFFER;
	case Buffer::Target::SHADER_STORAGE:
		return GL_SHADER_STORAGE_BUFFER;
	default:
		MM_CORE_UNINPLEMENTED();
	}
}

static GLenum ToGLTexTarget(Texture::Target target)
{
	switch (target) {
	case Texture::Target::TEXTURE_2D:
		return GL_TEXTURE_2D;
		break;
	default:
		MM_CORE_UNINPLEMENTED();
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
	case Graphics::TexFormat::D24S8:
		return GL_DEPTH24_STENCIL8;
		break;
	default:
		MM_CORE_UNINPLEMENTED();
	}
}

static GLenum ToGLPixelFormat(Graphics::PixelFormat format)
{
	switch (format) {
	case Graphics::PixelFormat::RGB:
		return GL_RGB;
	case Graphics::PixelFormat::RGBA:
		return GL_RGBA;
	default:
		MM_CORE_UNINPLEMENTED();
	}
}

static GLenum ToGLPixelType(Graphics::PixelType type)
{
	switch (type) {
	case Graphics::PixelType::UBYTE:
		return GL_UNSIGNED_BYTE;
		break;
	default:
		MM_CORE_UNINPLEMENTED();
	}
}

static GLenum ToGLAttachment(Graphics::AttachmentType attachment, u32 index)
{
	switch (attachment) {
	case Graphics::AttachmentType::DEPTH:
		return GL_DEPTH_ATTACHMENT;
		break;
	case Graphics::AttachmentType::COLOR:
		return GL_COLOR_ATTACHMENT0 + index;
		break;
	default:
		MM_CORE_UNINPLEMENTED();
	}
}

static GLenum ToGLShaderType(Graphics::ShaderType type)
{
	switch (type) {
	case Graphics::ShaderType::VERTEX:
		return GL_VERTEX_SHADER;
		break;
	case Graphics::ShaderType::FRAGMENT:
		return GL_FRAGMENT_SHADER;
		break;
	default:
		MM_CORE_UNINPLEMENTED();
	}
}

static GLenum ToGLIndexType(Graphics::IndexType type)
{
	switch (type) {
	case Graphics::IndexType::UBYTE:
		return GL_UNSIGNED_BYTE;
	case Graphics::IndexType::USHORT:
		return GL_UNSIGNED_SHORT;
	case Graphics::IndexType::UINT:
		return GL_UNSIGNED_INT;
	default:
		MM_CORE_UNREACHABLE();
	}
}

static GLenum ToGLTexWrap(Graphics::TexWrap wrap)
{
	switch (wrap) {
	case Graphics::TexWrap::REPEAT:
		return GL_REPEAT;
		break;
	case Graphics::TexWrap::CLAMP_TO_EDGE:
		return GL_CLAMP_TO_EDGE;
		break;
	case Graphics::TexWrap::CLAMP_TO_BORDER:
		return GL_CLAMP_TO_BORDER;
		break;
	default:
		MM_CORE_UNINPLEMENTED();
	}
}

static GLenum ToGLTexFilter(Graphics::TexFilter filter)
{
	switch (filter) {
	case Graphics::TexFilter::LINEAR:
		return GL_LINEAR;
		break;
	case Graphics::TexFilter::NEAREST:
		return GL_NEAREST;
		break;
	default:
		MM_CORE_UNINPLEMENTED();
	}
}

static GLenum ToGLBlendFactor(Graphics::BlendFactor fact)
{
	switch (fact) {
	case Graphics::BlendFactor::ONE_MINUS_SRC_ALPHA:
		return GL_ONE_MINUS_SRC_ALPHA;
	case Graphics::BlendFactor::SRC_ALPHA:
		return GL_SRC_ALPHA;
	default:
		MM_CORE_UNINPLEMENTED();
	}
}

static GLenum ToGLDrawMode(Graphics::DrawMode mode)
{
	switch (mode) {
	case Graphics::DrawMode::LINES:
		return GL_LINES;
	case Graphics::DrawMode::TRIANGLES:
		return GL_TRIANGLES;
	default:
		MM_CORE_UNINPLEMENTED();
	}
}

static GLbitfield ToGLBufferFlags(Graphics::BufferFlags flags)
{
	GLbitfield glflags{};
	u32 tmp = static_cast<u32>(flags);
	if (tmp & static_cast<u32>(Graphics::BufferFlags::MAP_READ_BIT))
		glflags |= GL_MAP_READ_BIT;
	if (tmp & static_cast<u32>(Graphics::BufferFlags::MAP_WRITE_BIT))
		glflags |= GL_MAP_WRITE_BIT;
	if (tmp & static_cast<u32>(Graphics::BufferFlags::DYNAMIC_STORAGE_BIT))
		glflags |= GL_DYNAMIC_STORAGE_BIT;
	return glflags;
}

static GLenum ToGLBufferAccess(Graphics::BufferAccess access)
{
	switch (access)
	{
	case Graphics::BufferAccess::READ:
		return GL_READ_ONLY;
	case Graphics::BufferAccess::WRITE:
		return GL_WRITE_ONLY;
	case Graphics::BufferAccess::READ_WRITE:
		return GL_READ_WRITE;
	default:
		MM_CORE_UNREACHABLE();
	}
}

void Graphics_GL::SetViewport(Vec2 size, Vec2 pos) const
{
	glViewport(pos.x, pos.y, size.x, size.y);
}

void Graphics_GL::SetFrontFace(FrontFace ff) const
{
	switch (ff) {
	case FrontFace::CW:
		glFrontFace(GL_CW);
		break;
	case FrontFace::CCW:
		glFrontFace(GL_CCW);
		break;
	default:
		MM_CORE_UNREACHABLE();
	}
}

void Graphics_GL::SetCulling(bool enable) const
{
	if (enable) 
		glEnable(GL_CULL_FACE);
	else 
		glDisable(GL_CULL_FACE);
}

void Graphics_GL::SetDepthTest(bool enable) const
{
	if (enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void Graphics_GL::SetBlend(bool enable) const
{
	if (enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}

void Graphics_GL::SetBlendFunc(BlendFactor src, BlendFactor dst) const
{
	glBlendFunc(ToGLBlendFactor(src), ToGLBlendFactor(dst));
}

void Graphics_GL::CreateBuffer(Buffer& buffer) const
{
	glCreateBuffers(1, buffer.GetIDPtr());
	MM_CORE_TRACE("GL: buffer created; id={0}", buffer.GetID());
}

void Graphics_GL::DeleteBuffer(Buffer& buffer) const
{
	glDeleteBuffers(1, buffer.GetIDPtr());
	MM_CORE_TRACE("GL: buffer deleted; id={0}", buffer.GetID());
}

void Graphics_GL::SetBufferStorage(const Buffer& buffer, const void* data, u32 size, BufferFlags flags) const
{
	// TODO: flags conversion 
	glNamedBufferStorage(buffer.GetID(), size, data, ToGLBufferFlags(flags));
}

void Graphics_GL::SetBufferSubData(const Buffer& buffer, const void* data, u32 size, u32 offset) const
{
	glNamedBufferSubData(buffer.GetID(), offset, size, data);
}

void Graphics_GL::SetBufferBindBase(const Buffer& buffer, u32 base) const
{
	glBindBufferBase(ToGLBufferTarget(buffer.GetTarget()), base, buffer.GetID());
}

void* Graphics_GL::MapBuffer(const Buffer& buffer, BufferAccess access) const
{
	return glMapNamedBuffer(buffer.GetID(), ToGLBufferAccess(access));
}

void Graphics_GL::UnmapBuffer(const Buffer& buffer) const
{
	glUnmapNamedBuffer(buffer.GetID());
}

void Graphics_GL::CreateVertexArray(VertexArray& va) const
{
	glCreateVertexArrays(1, va.GetIDPtr());
	MM_CORE_TRACE("GL: vertex array created; id={0}", va.GetID());
}

void Graphics_GL::DeleteVertexArray(VertexArray& va) const
{
	glDeleteVertexArrays(1, va.GetIDPtr());
	MM_CORE_TRACE("GL: vertex array deleted; id={0}", va.GetID());
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
	default:
		MM_CORE_UNINPLEMENTED();
	}

	glVertexArrayAttribBinding(va.GetID(), location, 0);
}

// TODO: Add type
void Graphics_GL::DrawElements(const VertexArray& va, u32 first, u32 count) const
{
	glBindVertexArray(va.GetID());
	const auto& ib = va.GetIndexBuffer();
	glDrawElements(GL_TRIANGLES, count, ToGLIndexType(ib.GetIndexType()),
				   reinterpret_cast<void*>(first * ib.GetIndexSize()));
}

void Graphics_GL::DrawArrays(DrawMode mode, u32 begin, u32 count) const
{
	glDrawArrays(ToGLDrawMode(mode), begin, count);
}

void Graphics_GL::CreateTexture(Texture& tex) const
{
	glCreateTextures(ToGLTexTarget(tex.GetTarget()), 1, tex.GetIDPtr());
	MM_CORE_TRACE("GL: texture created; id={0}", tex.GetID());
}

void Graphics_GL::DeleteTexture(Texture& tex) const
{
	glDeleteTextures(1, tex.GetIDPtr());
	MM_CORE_TRACE("GL: texture deleted; id={0}", tex.GetID());
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
									PixelFormat format, u32 level, 
									u32 xoffset, u32 yoffset) const
{
	glTextureSubImage2D(tex.GetID(), level, xoffset, yoffset, width, height, ToGLPixelFormat(format), ToGLPixelType(type), data);
}

void Graphics_GL::TextureStorage3D(const Texture& tex, 
								   u32 width, u32 height, u32 depth, 
								   TexFormat format, u32 levels) const
{
	glTextureStorage3D(tex.GetID(), levels, ToGLTexFormat(format),
					   width, height, depth);
}

void Graphics_GL::TextureSubImage3D(const Texture& tex, 
									const void* data, 
									PixelType type, 
									u32 width, u32 height, u32 depth, 
									PixelFormat format, 
									u32 level, 
									u32 xoffset, u32 yoffset, u32 zoffset) const
{
	glTextureSubImage3D(tex.GetID(), level,
						xoffset, yoffset, zoffset,
						width, height, depth,
						ToGLPixelFormat(format), 
						ToGLPixelType(type), 
						data); 
}

void Graphics_GL::SetTextureFilter(const Texture& tex, TexFilter min, TexFilter mag) const
{
	glTextureParameteri(tex.GetID(), GL_TEXTURE_MIN_FILTER, ToGLTexFilter(min));
	glTextureParameteri(tex.GetID(), GL_TEXTURE_MAG_FILTER, ToGLTexFilter(mag));
}

void Graphics_GL::SetTextureWrap2D(const Texture2D& tex, TexWrap u, TexWrap v) const
{
	glTextureParameteri(tex.GetID(), GL_TEXTURE_WRAP_S, ToGLTexWrap(u));
	glTextureParameteri(tex.GetID(), GL_TEXTURE_WRAP_T, ToGLTexWrap(v));
}

void Graphics_GL::CreateFrameBuffer(FrameBuffer& fb) const
{
	glCreateFramebuffers(1, fb.GetIDPtr());
	MM_CORE_TRACE("GL: frame buffer created; id={0}", fb.GetID());
}

void Graphics_GL::DeleteFrameBuffer(FrameBuffer& fb) const
{
	glDeleteFramebuffers(1, fb.GetIDPtr());
	MM_CORE_TRACE("GL: frame buffer deleted; id={0}", fb.GetID());
}

void Graphics_GL::FrameBufferTexture(const FrameBuffer& fb, const Texture& tex, AttachmentType attachment, u32 index, u32 level) const
{
	glNamedFramebufferTexture(fb.GetID(), ToGLAttachment(attachment, index), tex.GetID(), level);
}

Graphics::FrameBufferStatus Graphics_GL::CheckFrameBufferStatus(const FrameBuffer& fb) const
{
	GLenum status = glCheckNamedFramebufferStatus(fb.GetID(), GL_FRAMEBUFFER);
	switch (status) {
	case GL_FRAMEBUFFER_COMPLETE:
		return Graphics::FrameBufferStatus::OK;
		break;
	default:
		return Graphics::FrameBufferStatus::INCOMPLETE;
		break;
	}
}

void Graphics_GL::ClearFrameBufferColor(const FrameBuffer& fb, u32 index, Color color) const
{
	glClearNamedFramebufferfv(fb.GetID(), GL_COLOR, index, glm::value_ptr(color));
}

void Graphics_GL::ClearFrameBufferDepth(const FrameBuffer& fb, f32 depth, i32 stencil) const
{
	glClearNamedFramebufferfi(fb.GetID(), GL_DEPTH_STENCIL, 0, depth, stencil);
}

void Graphics_GL::BindFrameBuffer(const FrameBuffer& fb) 
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_lastFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, fb.GetID());
}

void Graphics_GL::UnbindFrameBuffer() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_lastFramebuffer);
}

void Graphics_GL::BindWindowFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Opt<String> Graphics_GL::CreateShader(Shader& shader, StringView source, ShaderType type) const
{
	*shader.GetIDPtr() = glCreateShader(ToGLShaderType(type));
	u32 id = shader.GetID();
	const char* raw = source.data();
	glShaderSource(id, 1, &raw, nullptr);
	glCompileShader(id);

	GLint ok{};
	glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
	if (!ok) {
		GLint len{};
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
		String msg{};
		msg.resize(len);
		glGetShaderInfoLog(id, len, nullptr, &msg[0]);
		return msg;
	} else {
		MM_CORE_TRACE("GL: shader created; id={0}", id);
		return std::nullopt;
	}
}

void Graphics_GL::DeleteShader(Shader& shader) const
{
	glDeleteShader(shader.GetID());
	MM_CORE_TRACE("GL: shader deleted; id={0}", shader.GetID());
}

void Graphics_GL::AttachShader(const Program& program, const Shader& shader) const
{
	glAttachShader(program.GetID(), shader.GetID());
}

void Graphics_GL::CreateProgram(Program& program) const
{
	*program.GetIDPtr() = glCreateProgram();
	MM_CORE_TRACE("GL: program created; id={0}", program.GetID());
}

void Graphics_GL::DeleteProgram(Program& program) const
{
	glDeleteProgram(program.GetID());
	MM_CORE_TRACE("GL: program deleted; id={0}", program.GetID());
}

Opt<String> Graphics_GL::LinkProgram(const Program& program) const
{
	u32 id = program.GetID();
	glLinkProgram(id);

	GLint ok{};
	glGetProgramiv(id, GL_LINK_STATUS, &ok);
	if (!ok) {
		GLint len{};
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
		String msg{};
		msg.resize(len);
		glGetProgramInfoLog(id, len, nullptr, &msg[0]);
		return msg;
	} else {
		return std::nullopt;
	}
}

void Graphics_GL::UseProgram(const Program& program) const
{
	glUseProgram(program.GetID());
}

u32 Graphics_GL::GetUniformCount(const Program& program) const
{
	i32 count{};
	glGetProgramiv(program.GetID(), GL_ACTIVE_UNIFORMS, &count);
	return count;
}

String Graphics_GL::GetUniformName(const Program& program, u32 index) const
{
	char buf[64]{};
	glGetActiveUniformName(program.GetID(), index, sizeof(buf), nullptr, buf);
	return String{ buf };
}

i32 Graphics_GL::GetUniformLocation(const Program& program, StringView name) const
{
	return glGetUniformLocation(program.GetID(), name.data());
}

void Graphics_GL::SetUniform(const Program& program, i32 location, u32 count, const f32* val) const
{
	glProgramUniform1fv(program.GetID(), location, count, val);
}

void Graphics_GL::SetUniform(const Program& program, i32 location, u32 count, const Vec2* val) const
{
	glProgramUniform2fv(program.GetID(), location, count, glm::value_ptr(*val));
}

void Graphics_GL::SetUniform(const Program& program, i32 location, u32 count, const Vec3* val) const
{
	glProgramUniform3fv(program.GetID(), location, count, glm::value_ptr(*val));
}

void Graphics_GL::SetUniform(const Program& program, i32 location, u32 count, const Vec4* val) const
{
	glProgramUniform4fv(program.GetID(), location, count, glm::value_ptr(*val));
}

void Graphics_GL::SetUniform(const Program& program, i32 location, u32 count, const i32* val) const
{
	glProgramUniform1iv(program.GetID(), location, count, val);
}

void Graphics_GL::SetUniform(const Program& program, i32 location, u32 count, const Vec2i* val) const
{
	glProgramUniform2iv(program.GetID(), location, count, glm::value_ptr(*val));
}

void Graphics_GL::SetUniform(const Program& program, i32 location, u32 count, const Vec3i* val) const
{
	glProgramUniform3iv(program.GetID(), location, count, glm::value_ptr(*val));
}

void Graphics_GL::SetUniform(const Program& program, i32 location, u32 count, const Vec4i* val) const
{
	glProgramUniform4iv(program.GetID(), location, count, glm::value_ptr(*val));
}

void Graphics_GL::SetUniform(const Program& program, i32 location, u32 count, const u32* val) const
{
	glProgramUniform1uiv(program.GetID(), location, count, val);
}

void Graphics_GL::SetUniform(const Program& program, i32 location, u32 count, const Vec2u* val) const
{
	glProgramUniform2uiv(program.GetID(), location, count, glm::value_ptr(*val));
}

void Graphics_GL::SetUniform(const Program& program, i32 location, u32 count, const Vec3u* val) const
{
	glProgramUniform3uiv(program.GetID(), location, count, glm::value_ptr(*val));
}

void Graphics_GL::SetUniform(const Program& program, i32 location, u32 count, const Vec4u* val) const
{
	glProgramUniform4uiv(program.GetID(), location, count, glm::value_ptr(*val));
}

void Graphics_GL::SetUniform(const Program& program, i32 location, u32 count, const Mat4* val, bool transpose) const
{
	glProgramUniformMatrix4fv(program.GetID(), location, count, transpose, glm::value_ptr(*val));
}

}
