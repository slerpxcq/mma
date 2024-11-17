#include "EditorPch.hpp"
#include "EditorApplication.hpp"

#include "EditorLayer.hpp"

#include "KeyMap.hpp"

namespace mm
{

void EditorApplication::Startup()
{
	Application::Startup();
	SetEditorKeyMap(new KeyMap{});
	auto km = GetEditorKeyMap();
	km->Insert("TransformEdit_Translate", ImGuiKey_T);
	km->Insert("TransformEdit_Rotate", ImGuiKey_R);
	km->Insert("TransformEdit_Escape", ImGuiKey_Escape);
	km->Insert("TransformEdit_X", ImGuiKey_X);
	km->Insert("TransformEdit_Y", ImGuiKey_Y);
	km->Insert("TransformEdit_Z", ImGuiKey_Z);
	km->Insert("TransformEdit_Toggle", ImGuiKey_W);

	m_layerStack.Push<EditorLayer>(m_window);
}

void EditorApplication::Shutdown()
{
	delete GetEditorKeyMap();
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

