#include "EditorPch.hpp"
#include "PropertiesPanel.hpp"

#include "Core/SceneManager.hpp"
#include "Core/Node.hpp"

#include <imgui.h>

namespace mm
{

void PropertiesPanel::OnUpdate(f32 deltaTime)
{
	Panel::OnUpdate(deltaTime);
}

void PropertiesPanel::OnRender()
{
	auto sm = GetSceneManager();
	ImGui::Begin(m_name.c_str());
	Panel::OnBegin();
	Panel::OnEnd();
	ImGui::End();
}

void PropertiesPanel::Visit(Node& node)
{
}

}