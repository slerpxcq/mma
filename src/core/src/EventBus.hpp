#pragma once

#include <dexode/EventBus.hpp>

namespace mm
{

class EventBus
{
	friend class EventListener;
public:
	EventBus() :
		m_impl{ std::make_shared<dexode::EventBus>() } {}

	void Process() { m_impl->process(); }

	template <typename E>
	void Postpone(const E& e) { m_impl->postpone(e); }

private:
	std::shared_ptr<dexode::EventBus> m_impl;
};

class EventListener
{
public: 
	template <typename C, typename E>
	using MemFnPtr = void(C::*)(const E&);

public:
	EventListener(EventBus& bus) : 
		m_impl(bus.m_impl) {}

	template <typename C, typename E>
	void Listen(MemFnPtr<C, E>&& fn, C* self) { 
		m_impl.listen<E>(std::bind(fn, self, std::placeholders::_1));
	}

private:
	dexode::EventBus::Listener m_impl;
};

}

