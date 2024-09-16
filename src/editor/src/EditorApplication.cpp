#include "EditorPch.hpp"
#include "EditorApplication.hpp"

#include "Core/src/ImGuiLayer.hpp"

#include "Core/src/InputManager.hpp"

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

