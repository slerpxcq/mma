#include "mmpch.hpp"
#include "EditorLayer.hpp"

#include "Core/App/Application.hpp"

#include "Core/App/Core.hpp"
#include "Core/App/Event.hpp"

#include "Core/GL/GLRenderer.hpp"

#include <nfd.h>

namespace mm
{
    void EditorLayer::OnAttach()
    {
        m_world = std::make_unique<World>();
        m_viewport = std::make_unique<Viewport>(*this);
        m_poseEditor = std::make_unique<PoseEditor>(*this);
        m_keyframeEditor = std::make_unique<KeyframeEditor>(*this);
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(float deltaTime)
    {
        m_world->OnUpdate(deltaTime);
        m_viewport->OnUpdate(deltaTime);
        m_poseEditor->OnUpdate(deltaTime);
        m_keyframeEditor->OnUpdate(deltaTime);

        GLRenderer& renderer = GLRenderer::s_instance;
        m_viewport->OnRender(renderer);
    }

    void EditorLayer::OnUIRender()
    {
        ImGui::Begin("Editor");
        if (ImGui::Button("Load model")) {
            nfdchar_t* path = nullptr;
            nfdresult_t result = NFD_OpenDialog("pmx", nullptr, &path);
            if (result == NFD_OKAY) {
                Model* model = m_world->LoadModel(path);
                m_poseEditor->SetModel(model);
                m_keyframeEditor->SetModel(model);
            }
        }
        if (ImGui::Button("Load animation")) {
            nfdchar_t* path = nullptr;
            nfdresult_t result = NFD_OpenDialog("vmd", nullptr, &path);
            if (result == NFD_OKAY) {
                Model* model = m_poseEditor->GetModel();
                if (model != nullptr) {
                    model->LoadAnimation(path);
                }
            }
        }
        if (ImGui::Button("Reset physics")) {
            m_world->GetPhysicsWorld().Reset();
        }
        ImGui::End();

        m_viewport->OnUIRender();
        m_poseEditor->OnUIRender();
        m_keyframeEditor->OnUIRender();
    }
}
