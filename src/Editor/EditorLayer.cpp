#include "mmpch.hpp"
#include "EditorLayer.hpp"

#include "Core/Application.hpp"

#include "Core/Core.hpp"
#include "Core/Event.hpp"

#include "Core/GL/GLRenderer.hpp"

#include <nfd.h>

namespace mm
{
    void EditorLayer::OnAttach()
    {
        m_scene = std::make_unique<World>();
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(float deltaTime)
    {
        m_scene->OnUpdate(deltaTime);
        GLRenderer& renderer = Application::Instance().GetRenderer();
        m_scene->Render(renderer);
    }

    void EditorLayer::OnUIRender()
    {
        ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoResize);
        if (ImGui::Button("Load model")) {
            nfdchar_t* path = nullptr;
            nfdresult_t result = NFD_OpenDialog("pmx", nullptr, &path);
            if (result == NFD_OKAY)
				m_scene->LoadModel(path);
        }
        if (ImGui::Button("Reset physics")) {
            m_scene->GetPhysicsWorld().Reset();
        }
        ImGui::End();
    }
}
