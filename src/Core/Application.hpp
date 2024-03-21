#pragma once

#include "AppLayerStack.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>

#include <memory>

namespace mm 
{
	struct Event;
	class ImGuiLayer;

	class TestLayer : public AppLayer {
	public:
		void OnUIRender() override {
			ImGui::ShowDemoWindow();
		}
	};

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

		void PushLayer(std::unique_ptr<AppLayer> layer) {
			m_layerStack.PushLayer(std::move(layer));
		}

		void PushOverlay(std::unique_ptr<AppLayer> overlay) {
			m_layerStack.PushOverlay(std::move(overlay));
		}

		void OnWindowClose(Event& e);
		void OnWindowResize(Event& e);

	private:
		Application() :
			m_window(nullptr),
			m_running(true) {}

		void RegisterCallbacks();

	private:
		GLFWwindow* m_window;

		AppLayerStack m_layerStack;
		ImGuiLayer* m_imguiLayer;

		bool m_running;
		bool m_minimized;
	};
}

