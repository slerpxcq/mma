#include "EditorPch.hpp"
#include "MenuBar.hpp"

namespace mm
{

void MenuBar::OnUIRender()
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open")) {
			}
			if (ImGui::MenuItem("Save")) {
			}
			if (ImGui::MenuItem("Save As")) {
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit")) {
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
			}
			if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
			}
			if (ImGui::MenuItem("Copy", "Ctrl+C")) {
			}
			if (ImGui::MenuItem("Paste", "Ctrl+V")) {
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

}
