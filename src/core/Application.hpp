#pragma once

#include "Window/Window.hpp"
#include "WindowEvent.hpp"
#include "EventBus.hpp"
#include "Layer/LayerStack.hpp"

namespace mm
{

class Application 
{
public:
	virtual ~Application() = default;
	virtual i32 Run();
	virtual void Startup();
	virtual void Shutdown();

	Application(i32 argc = {}, char** argv = {}); 

	void OnWindowClosed(const WindowEvent::WindowClosed& e) { m_running = false; }

protected:
	virtual void NewFrame(f32 deltaTime) = 0;

private:
	void RegisterCallbacks();

protected:
	i32 m_argc{};
	char** m_argv{};

	Scoped<EventBus::Listener> m_listener{};
	Window m_window;
	LayerStack m_layerStack{};

	bool m_running{ true };
	bool m_minimized{ false };
};

}


