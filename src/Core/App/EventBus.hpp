#pragma once

#include <dexode/EventBus.hpp>

namespace mm
{

class EventBus  
{
public:
	static std::shared_ptr<dexode::EventBus>& GetPtr() { return s_instance; }
	static dexode::EventBus& Get() { return *s_instance; }
	static void Init() { s_instance = std::make_shared<dexode::EventBus>(); }
	static void DeInit() { s_instance.reset(); }

private:
	EventBus() {}
	~EventBus() {}

private:
	static inline std::shared_ptr<dexode::EventBus> s_instance;
};

}
