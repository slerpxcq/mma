#include "mmpch.hpp"
#include "GLRenderer.hpp"

#include "GLShader.hpp"
#include "GLContext.hpp"

namespace mm
{
	GLRenderer::GLRenderer(GLContext& context) :
		m_context(context),
		m_shader(nullptr),
		m_proj(glm::mat4(1.0f))
	{
	}

	void GLRenderer::UseShader(const GLShader* shader)
	{
		m_shader = shader;
		if (m_shader != nullptr)
			Submit(MM_WRAP(m_shader->Use()));
	}

	void GLRenderer::Submit(Command cmd)
	{
		m_cmdQueue.push_back(cmd);
	}

	void GLRenderer::Commit()
	{
		for (const auto& cmd : m_cmdQueue) 
			cmd();

		m_cmdQueue.clear();
	}
}
