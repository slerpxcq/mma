#include "EditorPch.hpp"
#include "EditorApplication.hpp"

#include "Core/ImGuiLayer.hpp"
#include "Core/InputManager.hpp"

#include "Core/Graphics/IndexBuffer.hpp"

namespace mm
{

void EditorApplication::Startup()
{
	Application::Startup();

	/* BEGIN TEST CODE */
	
	/* END TEST CODE */

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

MM_APP_ENTRY(EditorApplication)

