#include "CorePch.hpp"
#include "WindowImpl_GLFW.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EventBus.hpp"
#include "WindowEvent.hpp"

#include "InputManager.hpp"

namespace mm
{

WindowImpl_GLFW::WindowImpl_GLFW(const Window::ConstructInfo& info) :
	Impl{ info }
{
	MM_ASSERT((info.api == GraphicsAPI::GL4) && "Vulkan is not implemented yet");

	if (!glfwInit()) {
		throw MMException("Could not init GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MM_GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MM_GL_VERSION_MINOR);

	m_window = glfwCreateWindow(info.width, info.height, info.title.data(), nullptr, nullptr);
	if (!m_window) {
		throw MMException("Could not create window");
	}

	glfwMakeContextCurrent(m_window);
	RegisterWindowEvents();

	if (!gladLoadGL()) {
		throw MMException("Could not load GL");
	}
}

WindowImpl_GLFW::~WindowImpl_GLFW()
{
	glfwTerminate();
}

void WindowImpl_GLFW::BeginFrame()
{
	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void WindowImpl_GLFW::EndFrame()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void WindowImpl_GLFW::RegisterWindowEvents()
{
	glfwSetWindowUserPointer(m_window, this);

	glfwSetErrorCallback([](int code, const char* what) {
		MM_ERROR("GLFW error:\n  Code: {0}\n  Info: {1}\n", code, what);
	});

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int w, int h) {
		InputManager::GetEventBus().Postpone<WindowEvent::WindowSized>({(uint32_t)w, (uint32_t)h});
	});

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
		InputManager::GetEventBus().Postpone<WindowEvent::WindowClosed>({});
	});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto& eb = InputManager::GetEventBus();
		switch (action) {
		case GLFW_PRESS:
			eb.Postpone<WindowEvent::KeyPressed>({(uint32_t)key, (uint32_t)mods, false});
			break;
		case GLFW_RELEASE:
			eb.Postpone<WindowEvent::KeyReleased>({(uint32_t)key});
			break;
		case GLFW_REPEAT:
			eb.Postpone<WindowEvent::KeyPressed>({ (uint32_t)key, (uint32_t)mods, true });
			break;
		}
	});

	glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
		InputManager::GetEventBus().Postpone<WindowEvent::KeyTyped>({(uint32_t)codepoint});
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
		switch (action) {
		case GLFW_PRESS:
			InputManager::GetEventBus().Postpone<WindowEvent::MouseButtonPressed>({ (uint32_t)button });
			break;
		case GLFW_RELEASE:
			InputManager::GetEventBus().Postpone<WindowEvent::MouseButtonReleased>({ (uint32_t)button });
			break;
		}
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
		InputManager::GetEventBus().Postpone<WindowEvent::MouseMoved>({ (float)xpos, (float)ypos });
	});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
		InputManager::GetEventBus().Postpone<WindowEvent::MouseScrolled>({ (float)yoffset });
	});
}

}

