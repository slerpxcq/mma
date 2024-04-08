#include "mmpch.hpp"
#include "MenuBarLayer.hpp"

#include "Core/App/Application.hpp"
#include "Core/App/Event.hpp"

#include <nfd.h>

namespace mm
{
	void MenuBarLayer::OnAttach()
	{
		m_appEventBus = Application::Instance().GetEventBus();
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
					m_appEventBus->postpone<Event::WindowClosed>({});
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}

