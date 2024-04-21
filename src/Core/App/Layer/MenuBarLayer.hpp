#pragma once

#include "Layer.hpp"

#include <dexode/EventBus.hpp>

namespace mm
{
	class MenuBarLayer : public Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnUIRender() override;

	private:
		std::shared_ptr<dexode::EventBus> m_appEventBus;
	};
}

