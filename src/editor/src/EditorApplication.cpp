#include "EditorPch.hpp"
#include "EditorApplication.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace mm
{

void EditorApplication::NewFrame(float deltaTime)
{
	m_GUIContext->Begin();

	m_viewport->OnUIRender();
	m_menuBar->OnUIRender();

	m_GUIContext->End();
}

void EditorApplication::Startup()
{
	Application::Startup();
	m_eventBus = std::make_shared<dexode::EventBus>();
	m_GUIContext = std::make_unique<ImGuiContext>(m_window);
	m_menuBar = std::make_unique<MenuBar>();
	m_viewport = std::make_unique<Viewport>();
}

void EditorApplication::Shutdown()
{
	Application::Shutdown();
}

void EditorApplication::RegisterWindowCallbacks()
{
	glfwSetWindowUserPointer(m_window, this);
}

}

