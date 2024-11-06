#include "EditorPch.hpp"
#include "MenuBarPanel.hpp"

#include <imgui.h>

namespace mm
{

void MenuBarPanel::OnUpdate(f32 deltaTime)
{
}

void MenuBarPanel::OnRender()
{
	ImGui::BeginMainMenuBar();
	ImGui::MenuItem("File");
	ImGui::MenuItem("Edit");
	ImGui::EndMainMenuBar();
}

}
