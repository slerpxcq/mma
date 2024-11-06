#include "CorePch.hpp"
#include "LayerStack.hpp"

namespace mm
{

void LayerStack::OnUpdate(float deltaTime)
{
	for (auto& layer : m_layers) {
		layer->OnUpdate(deltaTime);
	}
}

void LayerStack::OnRender()
{
	for (auto& layer : m_layers) {
		layer->OnRender();
	}
}

}
