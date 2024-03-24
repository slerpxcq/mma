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

	void LayerStack::OnUIRender() 
	{
		for (auto& l : m_layers) {
			l->OnUIRender();
		}
	}
}

