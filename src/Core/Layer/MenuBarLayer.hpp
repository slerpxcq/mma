#pragma once

#include "Layer.hpp"
#include "Core/EventUser.hpp"

#include <dexode/EventBus.hpp>

namespace mm
{
	class MenuBarLayer : public Layer, public EventUser
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnUIRender() override;
	};
}

