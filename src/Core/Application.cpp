#include "mmpch.hpp"
#include "Application.hpp"

#include "Core.hpp"
#include "Event.hpp"
#include "Layer/ImGuiLayer.hpp"
#include "Editor/EditorLayer.hpp"

#include "GL/GLShader.hpp"
#include "GL/GLBuffer.hpp"
#include "GL/GLVertexArray.hpp"
#include "GL/GLLayout.hpp"
#include "GL/GLTexture.hpp"
#include "SampleLayout.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace mm
{
	void Application::Init()
	{
		MM_INFO("App start");

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MM_GL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MM_GL_VERSION_MINOR);
		m_window = glfwCreateWindow(1280, 720, "", nullptr, nullptr);
		MM_ASSERT(m_window);
		RegisterCallbacks();

		m_glctx = std::make_unique<GLContext>(m_window);
		m_renderer = std::make_unique<Renderer>();

		auto imguiLayer = std::make_unique<ImGuiLayer>();
		m_imguiLayer = static_cast<ImGuiLayer*>(imguiLayer.get());
		PushOverlay(std::move(imguiLayer));

		PushLayer(std::make_unique<EditorLayer>());
	}

	void Application::RegisterCallbacks()
	{
		glfwSetWindowUserPointer(m_window, this);

		glfwSetErrorCallback([](int code, const char* what) {
			// FIXME: causing segfault
			//MM_ERROR("GLFW error:\n  Code: {0}\n  Info: {1}\n", code, what);
		});

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int w, int h) {
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			Event e = {};
			e.source = Event::Source::WINDOW;
			e.type = Event::Type::WINDOW_RESIZED;
			e.data.windowResize.w = w;
			e.data.windowResize.h = h;
			app->OnEvent(e);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			Event e = {};
			e.source = Event::Source::WINDOW;
			e.type = Event::Type::WINDOW_CLOSED;
			app->OnEvent(e);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			Event e = {};
			e.source = Event::Source::KEYBOARD;
			switch (action) {
			case GLFW_PRESS:
				e.type = Event::Type::KEY_PRESSED;
				e.data.key.repeat = false;
				break;
			case GLFW_RELEASE:
				e.type = Event::Type::KEY_RELEASED;
				e.data.key.repeat = false;
				break;
			case GLFW_REPEAT:
				e.type = Event::Type::KEY_PRESSED;
				e.data.key.repeat = true;
				break;
			}
			e.data.key.code = key;
			app->OnEvent(e);
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			Event e = {};
			e.source = Event::Source::KEYBOARD;
			e.type = Event::Type::KEY_TYPED;
			e.data.key.code = codepoint;
			app->OnEvent(e);
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			Event e = {};
			e.source = Event::Source::MOUSE_BUTTON;
			switch (action) {
			case GLFW_PRESS:
				e.type = Event::Type::MOUSE_BUTTON_PRESSED;
				break;
			case GLFW_RELEASE:
				e.type = Event::Type::MOUSE_BUTTON_RELEASED;
				break;
			}
			e.data.mouseButton.code = button;
			app->OnEvent(e);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			Event e = {};
			e.source = Event::Source::MOUSE;
			e.type = Event::Type::MOUSE_MOVED_;
			e.data.mouseMove.x = static_cast<float>(xpos);
			e.data.mouseMove.y = static_cast<float>(ypos);
			app->OnEvent(e);
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
			Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
			Event e = {};
			e.source = Event::Source::MOUSE;
			e.type = Event::Type::MOUSE_SCROLLED;
			e.data.mouseScroll.delta = static_cast<float>(yoffset);
			app->OnEvent(e);
		});
	}

	void Application::Run()
	{
		static MM_TIMEPOINT lastTime = MM_TIME_NOW();

		GLTexture tex("resources/textures/H.png", GL_TEXTURE_2D);
		
		GLShader shader;
		shader.Compile("resources/shaders/test.vert", GLShader::VERTEX);
		shader.Compile("resources/shaders/test.frag", GLShader::FRAGMENT);
		shader.Link();

		SampleLayout::Vertex triangle[] = {
			{ glm::vec2(-.5f, -.5f), glm::vec4(1, 0, 0, 1) },
			{ glm::vec2(0.f, .5f), glm::vec4(0, 1, 0, 1) },
			{ glm::vec2(.5f, -.5f), glm::vec4(0, 0, 1, 1) }
		};

		GLBuffer buffer(GL_ARRAY_BUFFER);
		buffer.Data(sizeof(triangle), triangle);

		std::unique_ptr<GLLayout> layout = std::make_unique<SampleLayout>();

		GLVertexArray varr;
		varr.Bind();
		varr.BindBuffer(buffer);
		varr.BufferLayout(layout.get());

		while (m_running) {
			float deltaTime = MM_TIME_DELTA(lastTime);
			lastTime = MM_TIME_NOW();

			m_glctx->Clear();

			if (!m_minimized) {
				m_layerStack.OnUpdate(deltaTime);
				
				m_imguiLayer->Begin();
				m_layerStack.OnUIRender();
				m_imguiLayer->End();
			}

			glm::vec4 color(0.f, 1.f, 0.f, 1.f);
			int32_t unit = 0;

			//m_renderer->Submit(std::bind(glEnable, GL_DEPTH_TEST));
			m_renderer->Submit(MM_WRAP(shader.Use()));
			m_renderer->Submit(MM_WRAP(tex.Bind(GL_TEXTURE0)));
			m_renderer->Submit(MM_WRAP(shader.Uniform("c", 1, &color)));
			m_renderer->Submit(MM_WRAP(shader.Uniform("tex", 1, &unit)));
			m_renderer->Submit(MM_WRAP(varr.DrawArray(GL_TRIANGLES, 0, 3)));
			m_renderer->Commit();

			m_glctx->SwapBuffers();
			glfwPollEvents();
		}
	}

	void Application::DeInit()
	{
		glfwTerminate();
	}

	void Application::OnEvent(Event& e)
	{
		switch (e.type) {
		case Event::Type::WINDOW_CLOSED:
			OnWindowClose(e);
			break;
		case Event::Type::WINDOW_RESIZED:
			OnWindowResize(e);
			break;
		default:
			m_layerStack.OnEvent(e);
			break;
		}
	}

	void Application::OnWindowClose(Event& e)
	{
		m_running = false;
		e.handled = true;
	}

	void Application::OnWindowResize(Event& e)
	{
		m_minimized = (e.data.windowResize.w == 0 || e.data.windowResize.h == 0);
		m_glctx->Viewport(e.data.windowResize.w, e.data.windowResize.h);
		e.handled = true;
	}
}
