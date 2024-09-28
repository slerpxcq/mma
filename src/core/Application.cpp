#include "CorePch.hpp"
#include "Application.hpp"

#include "InputManager.hpp"

#include "Common/Clock.hpp"
#include "Globals.hpp"

namespace mm
{

int Application::Run()
{
	Startup();

	static TimePoint tp = Clock::Now();
	while (m_running) {
		m_window.BeginFrame();
		NewFrame(Clock::Delta(tp));
		m_window.EndFrame();
		InputManager::GetEventBus().Process();
	}

	Shutdown();

	return 0;
}

void Application::Startup()
{
	/* TODO: */
	SetCurrentAPI(Config::GraphicsAPI::GL4);

	InputManager::CreateInstance();
	RegisterCallbacks();
}

void Application::Shutdown()
{
	InputManager::DestroyInstance();
}

void Application::RegisterCallbacks()
{
	m_listener = std::make_unique<EventBus::Listener>(InputManager::GetEventBus());
	m_listener->Listen(&Application::OnWindowClosed, this);
}

}
