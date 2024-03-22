#include "mmpch.hpp"
#include "Renderer.hpp"

namespace mm
{
	Renderer::Renderer()
	{
	}

	void Renderer::Submit(Command cmd)
	{
		m_cmdQueue.push_back(cmd);
	}

	void Renderer::Commit()
	{
		for (const auto& cmd : m_cmdQueue) 
			cmd();

		m_cmdQueue.clear();
	}
}
