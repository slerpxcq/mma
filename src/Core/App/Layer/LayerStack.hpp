#pragma once

#include "Layer.hpp"

#include "Core/App/Core.hpp"
#include "Core/App/Event.hpp"

namespace mm
{
	class LayerStack
	{
	public:
		LayerStack() :
			m_layerInsertPos(0) {};

		void OnUpdate(float deltaTime);
		void OnUIRender();

		void PushLayer(std::unique_ptr<ILayer> layer) {
			layer->OnAttach();
			m_layers.insert(m_layers.begin() + m_layerInsertPos, std::move(layer));
			++m_layerInsertPos;
		}

		void PushOverlay(std::unique_ptr<ILayer> overlay) {
			overlay->OnAttach();
			m_layers.push_back(std::move(overlay));
		}

		void PopLayer() {
			MM_FATAL("TO BE IMPLEMENTED");
		}

		void PopOverlay() {
			MM_FATAL("TO BE IMPLEMENTED");
		}

		~LayerStack() {
			for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
				(*it)->OnDetach();
		}

	private:
		std::vector<std::unique_ptr<ILayer>> m_layers;
		uint32_t m_layerInsertPos;
	};
}


