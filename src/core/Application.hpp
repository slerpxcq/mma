#pragma once

#include "Window.hpp"
#include "WindowEvent.hpp"
#include "EventBus.hpp"
#include "LayerStack.hpp"

namespace mm
{

class Application 
{
public:
	virtual ~Application() = default;
	virtual int Run();
	virtual void Startup();
	virtual void Shutdown();

	void OnWindowClosed(const WindowEvent::WindowClosed& e) { m_running = false; }

protected:
	virtual void NewFrame(float deltaTime) = 0;

private:
	void RegisterCallbacks();

protected:
	Scoped<EventBus::Listener> m_listener{};
	Window m_window;
	LayerStack m_layerStack;

	bool m_running{ true };
	bool m_minimized{ false };
};

}


