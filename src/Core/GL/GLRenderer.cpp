#include "mmpch.hpp"
#include "GLRenderer.hpp"

namespace mm
{
	GLRenderer::GLRenderer()
	{
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
