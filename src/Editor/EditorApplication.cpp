#include "EditorPch.hpp"
#include "EditorApplication.hpp"

#include "EditorLayer.hpp"

namespace mm
{

void EditorApplication::Startup()
{
	Application::Startup();

	m_layerStack.EmplaceBack<EditorLayer>(m_window);
}

void EditorApplication::Shutdown()
{
	Application::Shutdown();
}

void EditorApplication::OnNewFrame(f32 deltaTime)
{
	m_layerStack.OnRender();
}

void EditorApplication::OnUpdate(f32 deltaTime)
{
	m_layerStack.OnUpdate(deltaTime);
}

}

MM_APP_ENTRY(EditorApplication)

