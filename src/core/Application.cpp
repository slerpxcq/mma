#include "CorePch.hpp"
#include "Application.hpp"

#include "InputManager.hpp"

#include "Common/Clock.hpp"
#include "Globals.hpp"

#include "Graphics/OpenGL/Graphics_GL.hpp"

#include "Node.hpp"

namespace mm
{

Application::Application(i32 argc, char** argv) :
	m_argc{ argc }, m_argv{ argv }, m_graphics{ []() {
		SetGraphics(new Graphics_GL{});
		return GetGraphics();
	}() }
{
}

i32 Application::Run()
{
	Startup();

	static TimePoint tp = Clock::Now();
	while (m_running) {
		m_window.BeginFrame();
		NewFrame(Clock::Delta(tp));
		m_window.EndFrame();
		GetInputManager()->GetEventBus().Process();
	}

	Shutdown();

	return 0;
}

void Application::Startup()
{
	SetCoreLogger(new Logger{ "Core" });
	MM_CORE_INFO("Application started");

	SetInputManager(new InputManager{});
	SetRootNode(new Node{ "ROOT" });

	RegisterCallbacks();
}

void Application::Shutdown()
{
	MM_CORE_INFO("Application shutting down...");

	delete GetRootNode();
	delete GetInputManager();
	delete GetCoreLogger();
}

void Application::RegisterCallbacks()
{
	m_listener = MakeScoped<EventBus::Listener>(GetInputManager()->GetEventBus());
	m_listener->Listen(&Application::OnWindowClosed, this);
}

}
