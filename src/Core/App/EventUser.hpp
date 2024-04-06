#pragma once

#include <dexode/EventBus.hpp>

namespace mm
{
	class EventUser {
	public:
		EventUser();
		~EventUser();

	protected:
		std::shared_ptr<dexode::EventBus> m_eventBus;
		dexode::EventBus::Listener m_listener;
	};
}
