#pragma once

#include "Core.hpp"
#include "AppLayer.hpp"
#include "Events.hpp"

#include <vector>
#include <memory>
#include <chrono>

namespace mm
{
	class AppLayerStack
	{
	public:
		AppLayerStack() :
			m_layerInsertPos(0) {};


		void OnEvent(Event& e);
		void OnUpdate(float deltaTime);
		void OnUIRender();

		void PushLayer(std::unique_ptr<AppLayer> layer) {
			layer->OnAttach();
			m_layers.insert(m_layers.begin() + m_layerInsertPos, std::move(layer));
			++m_layerInsertPos;
		}

		void PushOverlay(std::unique_ptr<AppLayer> overlay) {
			overlay->OnAttach();
			m_layers.push_back(std::move(overlay));
		}

		void PopLayer() {
			MM_FATAL("TO BE IMPLEMENTED");
		}

		void PopOverlay() {
			MM_FATAL("TO BE IMPLEMENTED");
		}

		~AppLayerStack() {
			for (auto it = m_layers.rend(); it != m_layers.rbegin(); ++it)
				(*it)->OnDetach();
		}

	private:
		std::vector<std::unique_ptr<AppLayer>> m_layers;
		uint32_t m_layerInsertPos;
	};
}


