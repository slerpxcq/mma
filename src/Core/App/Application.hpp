#pragma once

#include "Layer/LayerStack.hpp"

#include "Core/GL/GLContext.hpp"
#include "Core/GL/GLTexture.hpp"
#include "Core/GL/GLRenderer.hpp"
#include "Core/GL/GLShader.hpp"

#include <dexode/EventBus.hpp>

#include "Core/ResourceManager/ResourceManager.hpp"

struct GLFWwindow;

namespace mm 
{
	class ImGuiLayer;

	class Application 
	{
	public:
		static Application& Instance() {
			if (s_instance == nullptr)
				s_instance = new Application();
			return *s_instance;
		}

		void Init();
		void Run();
		void DeInit();

		GLFWwindow* GetWindow() { return m_window; }
		ImGuiLayer* GetImGuiLayer() const { return m_imguiLayer; }

		void PushLayer(std::unique_ptr<Layer> layer) { m_layerStack.PushLayer(std::move(layer)); }
		void PushOverlay(std::unique_ptr<Layer> overlay) { m_layerStack.PushOverlay(std::move(overlay)); }

	private:
		void LoadTextures();
		void LoadShaders();
		void OnWindowClose(const Event::WindowClosed& e);
		void OnWindowResize(const Event::WindowSized& e);
		void OnKeyPressed(const Event::KeyPressed& e);

		Application() {}

		void RegisterWindowCallbacks();
		void ListenEvents();

	private:
		static Application* s_instance;

		GLFWwindow* m_window = nullptr;
		std::unique_ptr<GLContext> m_glContext;
		glm::uvec2 m_viewportSize = glm::vec2(0);

		// Event
		std::unique_ptr<dexode::EventBus::Listener> m_listener;

		// Layers
		LayerStack m_layerStack;
		ImGuiLayer* m_imguiLayer = nullptr;

		bool m_running = true;
		bool m_minimized = false;
	};
}

