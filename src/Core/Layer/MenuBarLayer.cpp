#include "mmpch.hpp"
#include "MenuBarLayer.hpp"

#include "Core/Application.hpp"
#include "Core/Event.hpp"

#include <nfd.h>

namespace mm
{
	void MenuBarLayer::OnAttach()
	{
		MM_INFO("MenuBarLayer");
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
				if (ImGui::MenuItem("Open")) {
					nfdchar_t* path = nullptr;
					nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &path);
					m_eventBus->postpone<Event::FileOpened>({ (const char*)path });
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit")) {
					m_eventBus->postpone<Event::WindowClosed>({});
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}

