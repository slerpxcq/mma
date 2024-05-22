#include "mmpch.hpp"
#include "Application.hpp"

#include "Event.hpp"
#include "EventBus.hpp"
#include "Log.hpp"

#include "Editor/Editor.hpp"

#include "Core/Manager/FileManager.hpp"
#include "Core/Manager/PMXFile.hpp"

#include "UIContext.hpp"

namespace mm
{

Application::Application()
{
}

Application::~Application()
{
}

void Application::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MM_GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MM_GL_VERSION_MINOR);
	m_window = glfwCreateWindow(1280, 720, "", nullptr, nullptr);
	MM_ASSERT(m_window);
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);
	gladLoadGL();
	RegisterGLErrorCallbacks();
}

void Application::RegisterGLErrorCallbacks()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback([](GLenum source,
							  GLenum type,
							  GLuint id,
							  GLenum severity,
							  GLsizei length,
							  const GLchar* message,
							  const void* userParam) {
								  if (type == GL_DEBUG_TYPE_ERROR) {
									  MM_ERROR("GL error:\nWhat: {0}", message);
								  }
						   }, nullptr);
}

void Application::RegisterWindowCallbacks()
{
	glfwSetWindowUserPointer(m_window, this);

	glfwSetErrorCallback([](int code, const char* what) {
		MM_ERROR("GLFW error:\n  Code: {0}\n  Info: {1}\n", code, what);
	});

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int w, int h) {
		EventBus::Get()->postpone<Event::WindowSized>({ (uint32_t)w, (uint32_t)h });
	});

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
		EventBus::Get()->postpone<Event::WindowClosed>({});
	});

	//glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
	//	auto& eb = EventBus::Get();
	//	switch (action) {
	//	case GLFW_PRESS:
	//		eb->postpone<Event::KeyPressed>({(uint32_t)key, (uint32_t)mods, false});
	//		break;
	//	case GLFW_RELEASE:
	//		eb->postpone<Event::KeyReleased>({(uint32_t)key});
	//		break;
	//	case GLFW_REPEAT:
	//		eb->postpone<Event::KeyPressed>({ (uint32_t)key, (uint32_t)mods, true });
	//		break;
	//	}
	//});

	//glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
	//	EventBus::Get()->postpone<Event::KeyTyped>({(uint32_t)codepoint});
	//});

	//glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
	//	switch (action) {
	//	case GLFW_PRESS:
	//		EventBus::Get()->postpone<Event::MouseButtonPressed>({ (uint32_t)button });
	//		break;
	//	case GLFW_RELEASE:
	//		EventBus::Get()->postpone<Event::MouseButtonReleased>({ (uint32_t)button });
	//		break;
	//	}
	//});

	//glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
	//	EventBus::Get()->postpone<Event::MouseMoved>({ (float)xpos, (float)ypos });
	//});

	//glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
	//	EventBus::Get()->postpone<Event::MouseScrolled>({ (float)yoffset });
	//});
}

void Application::Start()
{
	/* Initialize components */
	EventBus::Init();
	FileManager::Init();
	Editor::Init();
	UIContext::Init();

	InitWindow();
	RegisterWindowCallbacks();
	UIContext::Get().Start();

	m_listener = std::make_unique<dexode::EventBus::Listener>(EventBus::Get());
	m_listener->listen<Event::WindowClosed>(MM_EVENT_FN(Application::OnWindowClose));
	m_listener->listen<Event::WindowSized>(MM_EVENT_FN(Application::OnWindowResize));
}

void Application::Shutdown()
{
	/* Deinitialize components */
	UIContext::Get().Shutdown();
	UIContext::DeInit();
	Editor::DeInit();
	FileManager::Init();
	glfwTerminate();
}

void Application::Run()
{
	MM_INFO("Application started.");
	Start();

	FileManager::Get().Load<PMXFile>("resources/model/つみ式ミクさん/000 ミクさん.pmx");

	/* Loop */
	static MM_TIMEPOINT lastTime = MM_TIME_NOW();
	while (m_running) {
		float deltaTime = MM_TIME_DELTA(lastTime);
		lastTime = MM_TIME_NOW();

		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, m_windowSize.x, m_windowSize.y);

		if (!m_minimized) {
			Editor::Get().OnUpdate(deltaTime);

			UIContext::Get().Begin();
			Editor::Get().OnUIRender();
			UIContext::Get().End();
		}

		glfwSwapBuffers(m_window);
		glfwPollEvents();
		EventBus::Get()->process();
	}

	Shutdown();
	MM_INFO("Application exited.");
}

void Application::OnWindowClose(const Event::WindowClosed& e)
{
	m_running = false;
}

void Application::OnWindowResize(const Event::WindowSized& e)
{
	m_minimized = (e.x == 0 || e.y == 0);
	m_windowSize = glm::uvec2(e.x, e.y);
}

void Application::OnKeyPressed(const Event::KeyPressed& e)
{
	/* Ctrl + Z */
	if ((e.code == GLFW_KEY_Z) && (e.mods & GLFW_MOD_CONTROL)) {
		EventBus::Get()->postpone<Event::Undo>({});
	}

	/* Ctrl + Y */
	if ((e.code == GLFW_KEY_Y) && (e.mods & GLFW_MOD_CONTROL)) {
		EventBus::Get()->postpone<Event::Redo>({});
	}
}

}

