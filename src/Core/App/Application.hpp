#pragma once

#include "Event.hpp"
#include "EventBus.hpp"

namespace mm
{

class Application {
public:
	void Run();
	static Application& Get() { return *s_instance; }
	static void Init() { s_instance = new Application(); }
	static void DeInit() { delete s_instance; };

private:
	Application();
	~Application();

	void OnWindowClose(const Event::WindowClosed& e);
	void OnWindowResize(const Event::WindowSized& e);
	void OnKeyPressed(const Event::KeyPressed& e);

	void Start();
	void Shutdown();
	void InitWindow();
	void RegisterGLErrorCallbacks();
	void RegisterWindowCallbacks();

private:
	static Application* s_instance;

	GLFWwindow* m_window = nullptr;
	glm::uvec2 m_windowSize = glm::vec2(0);

	bool m_running = true;
	bool m_minimized = false;

	std::unique_ptr<dexode::EventBus::Listener> m_listener;
};

}
