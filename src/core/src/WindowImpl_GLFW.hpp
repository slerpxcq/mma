#pragma once

#include "Window.hpp"


struct GLFWwindow;

namespace mm
{

class EventBus;

class WindowImpl_GLFW : public Window::Impl
{
public:
	WindowImpl_GLFW(const Window::ConstructInfo& info);
	~WindowImpl_GLFW();

	virtual void BeginFrame() override;
	virtual void EndFrame() override;

private:
	void RegisterWindowEvents();

private:
	GLFWwindow* m_window{};
};

}

