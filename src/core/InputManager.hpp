#pragma once

#include "EventBus.hpp"

namespace mm
{

class InputManager 
{
public:
	EventBus& GetEventBus() { return m_eventBus; }

private:
	EventBus m_eventBus;
};

}

