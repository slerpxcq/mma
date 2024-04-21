#pragma once

#include <dexode/EventBus.hpp>

namespace mm
{
	class EventBus 
	{
	public:
		static std::shared_ptr<dexode::EventBus> Instance() { 
			if (s_instance == nullptr)
				s_instance = std::make_shared<dexode::EventBus>();
			return s_instance; 
		}

	private:
		EventBus() {}

	private:
		static std::shared_ptr<dexode::EventBus> s_instance;
	};
}
