#include "CorePch.hpp"
#include "Application.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <renderer/src/Renderer.hpp>

namespace mm
{

void Application::Run()
{
	Startup();

	static MM_TIMEPOINT tp = MM_TIME_NOW();
	while (m_running) {
		NewFrame(MM_TIME_DELTA(tp));
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	Shutdown();
}

void Application::Startup()
{
	if (!glfwInit()) {
		throw MMException("Failed to initialize GLFW!");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MM_GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MM_GL_VERSION_MINOR);

	m_window = glfwCreateWindow(m_info.width, m_info.height, m_info.title.c_str(), nullptr, nullptr);

	if (!m_window) {
		throw MMException("Failed to create window!");
	}

	glfwMakeContextCurrent(m_window);
	if (!gladLoadGL()) {
		throw MMException("GLAD failed to load OpenGL!");
	}

	Renderer::Init();
}

void Application::Shutdown()
{
	Renderer::DeInit();
	glfwDestroyWindow(m_window);
}

}
