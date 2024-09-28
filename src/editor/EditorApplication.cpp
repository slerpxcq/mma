#include "EditorPch.hpp"
#include "EditorApplication.hpp"

#include "Core/ImGuiLayer.hpp"

#include "Core/InputManager.hpp"

namespace mm
{

void EditorApplication::Startup()
{
	Application::Startup();

	m_layerStack.EmplaceBack<ImGuiLayer>(m_window);
}

void EditorApplication::Shutdown()
{
	Application::Shutdown();
}

void EditorApplication::NewFrame(float deltaTime)
{
	m_layerStack.OnRender();
}

}

