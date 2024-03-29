#pragma once

#include "Layer/LayerStack.hpp"
#include "GL/GLContext.hpp"
#include "GL/GLTexture.hpp"
#include "GL/GLRenderer.hpp"

#include <dexode/EventBus.hpp>

struct GLFWwindow;

namespace mm 
{
	class ImGuiLayer;

	class Application 
	{
	public:
		static Application& Instance() {
			// NOTE: Must be on heap to ensure it is living longer than the logger
			// otherwise the logger will be dangled and segfault will happen.
			if (s_instance == nullptr)
				s_instance = new Application();
			return *s_instance;
		}

		GLFWwindow* GetWindow() {
			return m_window;
		}

		GLRenderer& GetRenderer() {
			return *m_renderer;
		}

		std::vector<std::unique_ptr<GLTexture>>& GetToons() {
			return m_toons;
		}

		void Init();
		void Run();
		void DeInit();

		void PushLayer(std::unique_ptr<Layer> layer) {
			m_layerStack.PushLayer(std::move(layer));
		}

		void PushOverlay(std::unique_ptr<Layer> overlay) {
			m_layerStack.PushOverlay(std::move(overlay));
		}

		std::shared_ptr<dexode::EventBus> GetEventBus() {
			return m_eventBus;
		}

		void LoadToons();

		void OnWindowClose(const Event::WindowClosed& e);
		void OnWindowResize(const Event::WindowSized& e);

	private:
		Application() :
			m_running(true),
			m_minimized(false) {}

		void RegisterWindowCallbacks();
		void ListenEvents();

	private:
		static Application* s_instance;
		GLFWwindow* m_window;
		std::unique_ptr<GLContext> m_glctx;

		std::shared_ptr<dexode::EventBus> m_eventBus;
		std::unique_ptr<dexode::EventBus::Listener> m_listener;

		// Layers
		LayerStack m_layerStack;
		ImGuiLayer* m_imguiLayer;

		std::unique_ptr<GLRenderer> m_renderer;
		std::vector<std::unique_ptr<GLTexture>> m_toons;

		bool m_running;
		bool m_minimized;
	};
}

