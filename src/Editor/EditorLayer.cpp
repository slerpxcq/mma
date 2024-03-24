#include "mmpch.hpp"
#include "EditorLayer.hpp"

#include "Core/Application.hpp"

#include "Core/Core.hpp"
#include "Core/Event.hpp"

#include <nfd.h>

namespace mm
{
    void EditorLayer::OnAttach()
    {
        m_scene = std::make_unique<Scene>();
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(float deltaTime)
    {
        m_scene->Render(Application::Instance()->GetRenderer());
    }

    void EditorLayer::OnUIRender()
    {
        ImGui::Begin("Editor");
        if (ImGui::Button("Load model")) {
            nfdchar_t* path = nullptr;
            nfdresult_t result = NFD_OpenDialog("pmx", nullptr, &path);
            if (result == NFD_OKAY)
				m_scene->LoadModel(path);
        }
        ImGui::End();
    }
}
