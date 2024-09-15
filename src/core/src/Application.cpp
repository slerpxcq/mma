#include "CorePch.hpp"
#include "Application.hpp"

#include "InputManager.hpp"

namespace mm
{

int Application::Run()
{
	Startup();

	static MM_TIMEPOINT tp = MM_TIME_NOW();
	while (m_running) {
		NewFrame(MM_TIME_DELTA(tp));
	}

	Shutdown();

	return 0;
}

void Application::Startup()
{
	InputManager::CreateInstance();
	RegisterCallbacks();
}

void Application::Shutdown()
{
	InputManager::DestroyInstance();
}

void Application::NewFrame(float deltaTime)
{
	m_window.BeginFrame();
	m_window.EndFrame();

	InputManager::GetEventBus().Process();
}

void Application::RegisterCallbacks()
{
	m_listener = std::make_unique<EventListener>(InputManager::GetEventBus());
	m_listener->Listen(MM_CALLBACK(Application::OnWindowClosed));
}

}
