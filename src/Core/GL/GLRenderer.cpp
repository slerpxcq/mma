#include "mmpch.hpp"
#include "GLRenderer.hpp"

#include "GLShader.hpp"
#include "GLContext.hpp"
#include "GLTexture.hpp"
#include "GLVertexArray.hpp"
#include "GLFrameBuffer.hpp"

namespace mm
{
	GLRenderer::GLRenderer(GLContext& context) :
		m_context(context)
	{
	}

	void GLRenderer::BeginShader(GLShader* shader)
	{
		if (shader != nullptr)
			shader->Use();
		else
			glUseProgram(0);

		m_shader = shader;
	}

	void GLRenderer::EndShader()
	{
		BeginShader(nullptr);
	}

	void GLRenderer::BeginFramebuffer(GLFrameBuffer* framebuffer)
	{
		if (framebuffer != nullptr)
			framebuffer->Bind();
		else
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_framebuffer = framebuffer;
	}

	void GLRenderer::EndFramebuffer()
	{
		BeginFramebuffer(nullptr);
	}

	void GLRenderer::BeginVertexArray(GLVertexArray* va)
	{
		if (va != nullptr)
			va->Bind();
		else
			glBindVertexArray(0);

		m_vertexArray = va;
	}

	void GLRenderer::EndVertexArray()
	{
		BeginVertexArray(nullptr);
	}

	void GLRenderer::Enable(uint32_t what)
	{
		glEnable(what);
	}

	void GLRenderer::Disable(uint32_t what)
	{
		glDisable(what);
	}

	void GLRenderer::Viewport(uint32_t x, uint32_t y)
	{
		glViewport(0, 0, x, y);
	}

	void GLRenderer::BlendFunc(uint32_t src, uint32_t dst)
	{
		glBlendFunc(src, dst); 
	}

	void GLRenderer::BindTexture(const GLTexture& texture, uint32_t slot)
	{
		texture.Bind(slot);
	}

	void GLRenderer::Draw(bool indexed, uint32_t mode, uint32_t offset, uint32_t count)
	{
		if (m_vertexArray != nullptr) {
			if (indexed)
				m_vertexArray->DrawElem(mode, offset, count);
			else
				m_vertexArray->DrawArray(mode, offset, count);
		}
		else {
			MM_WARN("{0}: no vertex array is bound", __FUNCTION__);
		}
	}

	void GLRenderer::Barrier(uint32_t bitmask)
	{
		glMemoryBarrier(bitmask);
	}

	void GLRenderer::CullFace(uint32_t face)
	{
		glCullFace(face);
	}

	void GLRenderer::FrontFace(uint32_t front)
	{
		glFrontFace(front);
	}

	void GLRenderer::Clear(const glm::vec4& color, uint32_t bitmask)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(bitmask); 
	}
}
