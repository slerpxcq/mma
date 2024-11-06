#include "CorePch.hpp"
#include "WindowImpl_GLFW.hpp"


#include "Application/InputManager.hpp"
#include "Application/EventBus.hpp"
#include "Application/WindowEvent.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

namespace mm
{

static void RegisterGLErrorCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback([](GLenum source,
							  GLenum type,
							  GLuint id,
							  GLenum severity,
							  GLsizei length,
							  const GLchar* message,
							  const void* userParam) {
								  switch (type) {
								  case GL_DEBUG_TYPE_ERROR:
									  MM_CORE_ERROR("GL: {0}", message);
									  break;
								  }
						   }, nullptr);
}

WindowImpl_GLFW::WindowImpl_GLFW(const Window::ConstructInfo& m_info) :
	Impl{ m_info }
{
	if (!glfwInit()) {
		throw RuntimeError("Could not init GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Config::GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Config::GL_VERSION_MINOR);

	m_window = glfwCreateWindow(m_info.width, m_info.height, m_info.title.data(), nullptr, nullptr);
	if (!m_window) {
		throw RuntimeError("Could not create window");
	}

	glfwMakeContextCurrent(m_window);
	RegisterWindowEvents();

	if (!gladLoadGL()) {
		throw RuntimeError("Could not load GL");
	}
}

WindowImpl_GLFW::~WindowImpl_GLFW()
{
	glfwTerminate();
}

void WindowImpl_GLFW::BeginFrame()
{
	glm::vec4 clearColor{ .2f, .2f, .2f, .2f };
	f32 clearDepth{ 0.f };
	i32 clearStencil{ 0 };
	glClearNamedFramebufferfv(0, GL_COLOR, 0, glm::value_ptr(clearColor));
	glClearNamedFramebufferfi(0, GL_DEPTH, 0, clearDepth, clearStencil);

	// glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	// glClear(GL_COLOR_BUFFER_BIT);
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
		//MM_ERROR("GLFW error:\n  Code: {0}\n  Info: {1}\n", code, what);
	});

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int w, int h) {
		GetInputManager()->GetEventBus().Postpone<WindowEvent::WindowSized>({(uint32_t)w, (uint32_t)h});
	});

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
		GetInputManager()->GetEventBus().Postpone<WindowEvent::WindowClosed>({});
	});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto& eb = GetInputManager()->GetEventBus();
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
		GetInputManager()->GetEventBus().Postpone<WindowEvent::KeyTyped>({(uint32_t)codepoint});
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
		switch (action) {
		case GLFW_PRESS:
			GetInputManager()->GetEventBus().Postpone<WindowEvent::MouseButtonPressed>({ (uint32_t)button });
			break;
		case GLFW_RELEASE:
			GetInputManager()->GetEventBus().Postpone<WindowEvent::MouseButtonReleased>({ (uint32_t)button });
			break;
		}
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
		GetInputManager()->GetEventBus().Postpone<WindowEvent::MouseMoved>({ (float)xpos, (float)ypos });
	});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
		GetInputManager()->GetEventBus().Postpone<WindowEvent::MouseScrolled>({ (float)yoffset });
	});
}

}

