#include "mmpch.hpp"
#include "GLRenderer.hpp"

#include "GLShader.hpp"
#include "GLContext.hpp"
#include "GLTexture.hpp"
#include "GLVertexArray.hpp"

namespace mm
{
	GLRenderer::GLRenderer(GLContext& context) :
		m_context(context)
	{
	}

	void GLRenderer::UseShader(GLShader& shader)
	{
		shader.Use();
		m_shader = &shader;
	}

	void GLRenderer::Begin(uint32_t what)
	{
		glEnable(what);
	}

	void GLRenderer::End(uint32_t what)
	{
		glDisable(what);
	}

	void GLRenderer::Viewport(uint32_t x, uint32_t y)
	{
		glViewport(0, 0, x, y);
	}

	void GLRenderer::BlendFunc(uint32_t src, uint32_t dst)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	}

	void GLRenderer::BindTexture(const GLTexture& texture, uint32_t slot)
	{
		texture.Bind(slot);
	}

	void GLRenderer::Draw(const GLVertexArray& va, bool indexed, uint32_t mode, uint32_t offset, uint32_t count)
	{
		va.Bind();

		if (indexed)
			va.DrawElem(mode, offset, count);
		else
			va.DrawArray(mode, offset, count);
	}

	void GLRenderer::Barrier(uint32_t bitmask)
	{
		glMemoryBarrier(bitmask);
	}

	void GLRenderer::Clear(const glm::vec4& color, uint32_t bitmask)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(bitmask); 
	}
}
