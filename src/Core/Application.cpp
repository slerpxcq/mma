#include "mmpch.hpp"
#include "Application.hpp"

#include "Core.hpp"
#include "Event.hpp"
#include "Layer/ImGuiLayer.hpp"
#include "Layer/MenuBarLayer.hpp"
#include "Editor/EditorLayer.hpp"

namespace mm
{
	Application* Application::s_instance = nullptr;

	void Application::Init()
	{
		MM_INFO("App started");

		// Window
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MM_GL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MM_GL_VERSION_MINOR);
		m_window = glfwCreateWindow(1280, 720, "", nullptr, nullptr);
		MM_ASSERT(m_window);

		// Event
		m_eventBus = std::make_unique<dexode::EventBus>();
		m_listener = std::make_unique<dexode::EventBus::Listener>(m_eventBus);
		RegisterWindowCallbacks();
		ListenEvents();

		m_glctx = std::make_unique<GLContext>(m_window);
		m_renderer = std::make_unique<GLRenderer>(*m_glctx);
		LoadToons();

		auto imguiLayer = std::make_unique<ImGuiLayer>();
		m_imguiLayer = imguiLayer.get();
		PushOverlay(std::move(imguiLayer));

		PushOverlay(std::make_unique<MenuBarLayer>());

		PushLayer(std::make_unique<EditorLayer>());
	}

	void Application::LoadToons()
	{
		constexpr uint32_t TOON_COUNT = 11;

		for (uint32_t i = 0; i < TOON_COUNT; ++i) {
			std::filesystem::path toonPath = "resources/textures/toon";
			toonPath += (i < 10) ? 
				'0' + std::to_string(i) :
				std::to_string(i);
			toonPath += ".bmp";
			m_toons.push_back(std::make_unique<GLTexture>(toonPath, GL_TEXTURE_2D));
		}
	}

	void Application::RegisterWindowCallbacks()
	{
		glfwSetWindowUserPointer(m_window, this);

		glfwSetErrorCallback([](int code, const char* what) {
			MM_ERROR("GLFW error:\n  Code: {0}\n  Info: {1}\n", code, what);
		});

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int w, int h) {
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			app->GetEventBus()->postpone<Event::WindowSized>({ (uint32_t)w, (uint32_t)h });
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			app->GetEventBus()->postpone<Event::WindowClosed>({});
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			auto eb = app->GetEventBus();
			switch (action) {
			case GLFW_PRESS:
				eb->postpone<Event::KeyPressed>({(uint32_t)key, false});
				break;
			case GLFW_RELEASE:
				eb->postpone<Event::KeyReleased>({(uint32_t)key});
				break;
			case GLFW_REPEAT:
				eb->postpone<Event::KeyPressed>({ (uint32_t)key, true });
				break;
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			app->GetEventBus()->postpone<Event::KeyTyped>({(uint32_t)codepoint});
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			switch (action) {
			case GLFW_PRESS:
				app->GetEventBus()->postpone<Event::MouseButtonPressed>({ (uint32_t)button });
				break;
			case GLFW_RELEASE:
				app->GetEventBus()->postpone<Event::MouseButtonReleased>({ (uint32_t)button });
				break;
			}
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			app->GetEventBus()->postpone<Event::MouseMoved>({ (float)xpos, (float)ypos });
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			app->GetEventBus()->postpone<Event::MouseScrolled>({ (float)yoffset });
		});
	}

	void Application::ListenEvents() 
	{
		m_listener->listen<Event::WindowClosed>(MM_EVENT_FN(Application::OnWindowClose));
		m_listener->listen<Event::WindowSized>(MM_EVENT_FN(Application::OnWindowResize));
	}

	void Application::Run()
	{
		static MM_TIMEPOINT lastTime = MM_TIME_NOW();

		while (m_running) {
			float deltaTime = MM_TIME_DELTA(lastTime);
			lastTime = MM_TIME_NOW();

			 m_glctx->Clear();

			if (!m_minimized) {
				m_layerStack.OnUpdate(deltaTime);
				m_renderer->Commit();
				
				m_imguiLayer->Begin();
				m_layerStack.OnUIRender();
				m_imguiLayer->End();
			}

			m_glctx->SwapBuffers();
			glfwPollEvents();
			m_eventBus->process();
		}
	}

	void Application::DeInit()
	{
		m_listener->unlistenAll();

		MM_ASSERT(s_instance);
		delete s_instance;

		glfwTerminate();

		MM_INFO("App exited");
	}

	void Application::OnWindowClose(const Event::WindowClosed& e)
	{
		m_running = false;
	}

	void Application::OnWindowResize(const Event::WindowSized& e)
	{
		m_minimized = (e.x == 0 || e.y == 0);
		m_glctx->Viewport(e.x, e.y);
	}
}
