#pragma once

#include "Window.hpp"

struct GLFWwindow;

namespace mm
{

class WindowImpl_GLFW : public Window::Impl
{
public:
	WindowImpl_GLFW(const Window::ConstructInfo& m_info);
	~WindowImpl_GLFW();

	virtual void BeginFrame() override;
	virtual void EndFrame() override;

	virtual void* GetHandle() const override { return static_cast<void*>(m_window); }

private:
	void RegisterWindowEvents();

private:
	GLFWwindow* m_window{};
};

}

