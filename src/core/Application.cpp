#include "CorePch.hpp"
#include "Application.hpp"

#include "InputManager.hpp"

#include "Common/Clock.hpp"
#include "Globals.hpp"

#include "Node.hpp"

namespace mm
{

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
	SetGraphicsAPI(Config::GraphicsAPI::GL4);

	SetCoreLogger(MakeScoped<Logger>("Core"));
	MM_CORE_INFO("Application started");

	SetInputManager(MakeScoped<InputManager>());

	RegisterCallbacks();
}

void Application::Shutdown()
{
	MM_CORE_INFO("Application shutting down...");
}

void Application::RegisterCallbacks()
{
	m_listener = MakeScoped<EventBus::Listener>(GetInputManager()->GetEventBus());
	m_listener->Listen(&Application::OnWindowClosed, this);
}

}
