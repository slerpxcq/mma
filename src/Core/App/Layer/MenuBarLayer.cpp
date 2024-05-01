#include "mmpch.hpp"
#include "MenuBarLayer.hpp"

#include "Core/App/Application.hpp"
#include "Core/App/EventBus.hpp"
#include "Core/App/Event.hpp"

#include <nfd.h>

namespace mm
{
	void MenuBarLayer::OnAttach()
	{
	}

	void MenuBarLayer::OnDetach()
	{
	}

	void MenuBarLayer::OnUpdate(float deltaTime)
	{
	}

	void MenuBarLayer::OnUIRender()
	{
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Exit")) {
					EventBus::Instance()->postpone<Event::WindowClosed>({});
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
					EventBus::Instance()->postpone<Event::Undo>({});
				}
				if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
					EventBus::Instance()->postpone<Event::Redo>({});
				}
				if (ImGui::MenuItem("Copy", "Ctrl+C")) {
					EventBus::Instance()->postpone<Event::Copy>({});
				}
				if (ImGui::MenuItem("Paste", "Ctrl+V")) {
					EventBus::Instance()->postpone<Event::Paste>({});
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}

