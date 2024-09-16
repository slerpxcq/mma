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
	virtual ~Application() {}
	virtual int Run();
	virtual void Startup();
	virtual void Shutdown();

	/* Event callbacks */
	void OnWindowClosed(const WindowEvent::WindowClosed& e) { m_running = false; }

protected:
	virtual void NewFrame(float deltaTime) {}

private:
	void RegisterCallbacks();

protected:
	std::unique_ptr<EventListener> m_listener{};
	Window m_window;
	LayerStack m_layerStack;

	bool m_running{ true };
	bool m_minimized{ false };
};

}


