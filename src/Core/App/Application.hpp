#pragma once

#include "Event.hpp"
#include "EventBus.hpp"

#include "Core/Utility/Singleton.hpp"

namespace mm
{

class Application : public Singleton<Application> 
{
	friend class Singleton<Application>;
	friend class UIContext;
public:
	void Run();

private:
	Application() {}

	void OnWindowClose(const Event::WindowClosed& e);
	void OnWindowResize(const Event::WindowSized& e);
	void OnKeyPressed(const Event::KeyPressed& e);

	virtual void Startup() override;
	virtual void Shutdown() override;
	void ShowMenuBar();
	void InitWindow();
	void RegisterGLErrorCallbacks();
	void RegisterWindowCallbacks();

private:
	GLFWwindow* m_window = nullptr;
	glm::uvec2 m_windowSize = glm::vec2(0);

	bool m_running = true;
	bool m_minimized = false;

	std::unique_ptr<dexode::EventBus::Listener> m_listener;
};

}
