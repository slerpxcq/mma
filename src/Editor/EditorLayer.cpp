#include "mmpch.hpp"
#include "EditorLayer.hpp"
#include "Core/Event.hpp"

namespace mm
{
    void EditorLayer::OnAttach()
    {
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(float deltaTime)
    {
    }

    void EditorLayer::OnUIRender()
    {
        ImGui::ShowDemoWindow();
    }

    void EditorLayer::OnEvent(Event& e)
    {
    }
}
