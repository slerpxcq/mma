#pragma once

#include "Layer/LayerStack.hpp"
#include "GL/GLContext.hpp"
#include "Renderer.hpp"

struct GLFWwindow;

namespace mm 
{
	struct Event;
	class ImGuiLayer;

	class Application {
	public:
		static Application& Instance() {
			static Application application;
			return application;
		}

		GLFWwindow* GetWindow() {
			return m_window;
		}

		void Init();
		void Run();
		void DeInit();
		void OnEvent(Event& e); 

		void PushLayer(std::unique_ptr<Layer> layer) {
			m_layerStack.PushLayer(std::move(layer));
		}

		void PushOverlay(std::unique_ptr<Layer> overlay) {
			m_layerStack.PushOverlay(std::move(overlay));
		}

		void OnWindowClose(Event& e);
		void OnWindowResize(Event& e);

	private:
		Application() :
			m_running(true) {}

		void RegisterCallbacks();

	private:
		GLFWwindow* m_window;
		std::unique_ptr<GLContext> m_glctx;

		LayerStack m_layerStack;
		ImGuiLayer* m_imguiLayer;

		std::unique_ptr<Renderer> m_renderer;

		bool m_running;
		bool m_minimized;
	};
}

