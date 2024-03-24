#include "mmpch.hpp"
#include "EventUser.hpp"

#include "Application.hpp"

namespace mm
{
	EventUser::EventUser() :
		m_eventBus(Application::Instance()->GetEventBus()),
		m_listener(m_eventBus) {
	}
}