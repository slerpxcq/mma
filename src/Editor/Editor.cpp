#include "mmpch.hpp"
#include "Editor.hpp"

namespace mm
{

void Editor::Startup()
{
}

void Editor::Shutdown()
{
}

void Editor::OnUpdate(float deltaTime)
{
}

void Editor::OnUIRender()
{
	m_propertiesPanel.OnUIRender();
	m_sceneHierarchyPanel.OnUIRender();
}

}

