#include "AppLayerStack.hpp"

namespace mm
{
	void AppLayerStack::OnUpdate(float deltaTime)
	{
		for (auto& l : m_layers) {
			l->OnUpdate(deltaTime);
		}
	}

	void AppLayerStack::OnEvent(Event& e) {
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it) {
			(*it)->OnEvent(e);
			if (e.handled)
				break;
		}
	}

	void AppLayerStack::OnUIRender() 
	{
		for (auto& l : m_layers) {
			l->OnUIRender();
		}
	}
}

