#pragma once

#include "EventBus.hpp"

namespace mm
{

class InputManager : public Singleton<InputManager>
{
public:
	static EventBus& GetEventBus() { return s_instance->m_eventBus; }

private:
	EventBus m_eventBus;
};

}

