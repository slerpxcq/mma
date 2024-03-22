#include "mmpch.hpp"
#include "LayerStack.hpp"

namespace mm
{
	void LayerStack::OnUpdate(float deltaTime)
	{
		for (auto& l : m_layers) {
			l->OnUpdate(deltaTime);
		}
	}

	void LayerStack::OnEvent(Event& e) {
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it) {
			(*it)->OnEvent(e);
			if (e.handled)
				break;
		}
	}

	void LayerStack::OnUIRender() 
	{
		for (auto& l : m_layers) {
			l->OnUIRender();
		}
	}
}

