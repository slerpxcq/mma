#include "CorePch.hpp"
#include "Application.hpp"

#include "InputManager.hpp"

#include "Common/Clock.hpp"
#include "Globals.hpp"

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
		GetCoreInputManager()->GetEventBus().Process();
	}

	Shutdown();

	return 0;
}

void Application::Startup()
{
	/* TODO: */
	SetGraphicsAPI(Config::GraphicsAPI::GL4);

	/* TODO: Use Scoped*/
	SetCoreLogger(new Logger{"Core"});
	MM_CORE_INFO("Application started");

	SetCoreInputManager(new InputManager{});

	RegisterCallbacks();
}

void Application::Shutdown()
{
	/* TODO: Use Scoped*/
	MM_CORE_INFO("Application shutting down...");
	delete GetCoreLogger();
	delete GetCoreInputManager();
}

void Application::RegisterCallbacks()
{
	m_listener = std::make_unique<EventBus::Listener>(GetCoreInputManager()->GetEventBus());
	m_listener->Listen(&Application::OnWindowClosed, this);
}

}
