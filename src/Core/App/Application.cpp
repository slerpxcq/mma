#include "mmpch.hpp"
#include "Application.hpp"

#include "Core.hpp"
#include "Event.hpp"
#include "Layer/ImGuiLayer.hpp"
#include "Layer/MenuBarLayer.hpp"
#include "Editor/EditorLayer.hpp"
#include "Core/GL/GLCubeMap.hpp"

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

		// context and renderer
		m_glContext = std::make_unique<GLContext>(m_window);
		GLRenderer::s_instance.Init();

		// Resources
		LoadTextures();
		LoadShaders();

		// Layers
		auto imguiLayer = std::make_unique<ImGuiLayer>();
		m_imguiLayer = imguiLayer.get();
		PushOverlay(std::move(imguiLayer));

		PushOverlay(std::make_unique<MenuBarLayer>());
		PushLayer(std::make_unique<EditorLayer>());
	}

	void Application::LoadShaders()
	{
		auto defaultShader = std::make_unique<GLShader>();
		defaultShader->Compile("resources/shaders/default.vert", GLShader::VERTEX);
		defaultShader->Compile("resources/shaders/default.frag", GLShader::FRAGMENT);
		defaultShader->Link();

		auto morphShader = std::make_unique<GLShader>();
		morphShader->Compile("resources/shaders/morph.vert", GLShader::VERTEX);
		morphShader->Link();

		auto quadShader = std::make_unique<GLShader>();
		quadShader->Compile("resources/shaders/quad.vert", GLShader::VERTEX);
		quadShader->Compile("resources/shaders/quad.frag", GLShader::FRAGMENT);
		quadShader->Link();

		auto skyboxShader = std::make_unique<GLShader>();
		skyboxShader->Compile("resources/shaders/skybox.vert", GLShader::VERTEX);
		skyboxShader->Compile("resources/shaders/skybox.frag", GLShader::FRAGMENT);
		skyboxShader->Link();

		auto gridShader = std::make_unique<GLShader>();
		gridShader->Compile("resources/shaders/grid.vert", GLShader::VERTEX);
		gridShader->Compile("resources/shaders/grid.frag", GLShader::FRAGMENT);
		gridShader->Link();

		ResourceManager::s_instance.LoadShader("default", std::move(defaultShader));
		ResourceManager::s_instance.LoadShader("morph", std::move(morphShader));
		ResourceManager::s_instance.LoadShader("quad", std::move(quadShader));
		ResourceManager::s_instance.LoadShader("skybox", std::move(skyboxShader));
		ResourceManager::s_instance.LoadShader("grid", std::move(gridShader));
	}

	void Application::LoadTextures()
	{
		constexpr uint32_t TOON_COUNT = 11;

		for (uint32_t i = 0; i < TOON_COUNT; ++i) {
			std::filesystem::path toonPath = "resources/textures/";
			std::filesystem::path name = "toon";
			// toon00.bmp, ..., toon10.bmp
			name += (i < 10) ? 
				'0' + std::to_string(i) :
				std::to_string(i);
			name += ".bmp";
			toonPath += name;
			ResourceManager::s_instance.LoadTexture(name.u8string(), std::make_unique<GLTexture2D>(toonPath));
		}

		ResourceManager::s_instance.LoadTexture("uv_test", std::make_unique<GLTexture2D>("resources/textures/uvTex.png"));

		GLCubeMapConstructInfo info;
		info.paths[0] = "resources/textures/skybox/right.jpg";
		info.paths[1] = "resources/textures/skybox/left.jpg";
		info.paths[2] = "resources/textures/skybox/top.jpg";
		info.paths[3] = "resources/textures/skybox/bottom.jpg";
		info.paths[4] = "resources/textures/skybox/front.jpg";
		info.paths[5] = "resources/textures/skybox/back.jpg";

		auto skybox = std::make_unique<GLCubeMap>(info);
		ResourceManager::s_instance.LoadTexture("skybox", std::move(skybox));
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

			glClearColor(0.1f, 0.1f, 0.1f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

			if (!m_minimized) {
				m_layerStack.OnUpdate(deltaTime);
				
				m_imguiLayer->Begin();
				m_layerStack.OnUIRender();
				m_imguiLayer->End();
			}

			m_glContext->SwapBuffers();
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
		m_viewportSize = glm::uvec2(e.x, e.y);
	}
}