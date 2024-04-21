#include "mmpch.hpp"
#include "EditorLayer.hpp"

#include "EditorEvent.hpp"

#include "Core/App/Application.hpp"

#include "Core/App/Core.hpp"
#include "Core/App/Event.hpp"

#include "Core/GL/GLRenderer.hpp"

#include <nfd.h>

namespace mm
{
    void EditorLayer::OnAttach()
    {
        m_listener = std::make_unique<dexode::EventBus::Listener>(Application::Instance().GetEventBus());

        m_world = std::make_unique<World>();
        m_viewport = std::make_unique<Viewport>(*this);
        m_poseEditor = std::make_unique<PoseEditor>(*this);
        m_sequencer = std::make_unique<Sequencer>(*this);
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(float deltaTime)
    {
        m_world->OnUpdate(deltaTime);
        m_viewport->OnUpdate(deltaTime);
        m_poseEditor->OnUpdate(deltaTime);
        m_sequencer->OnUpdate(deltaTime);

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
                m_model = model;
                Application::Instance().GetEventBus()->postpone<EditorEvent::ModelLoaded>({ model });
            }
        }
        if (ImGui::Button("Load animation")) {
            nfdchar_t* path = nullptr;
            nfdresult_t result = NFD_OpenDialog("vmd", nullptr, &path);
            if (result == NFD_OKAY) {
                if (m_model != nullptr) {
                    Animation* animation = m_model->LoadAnimation(path);
					Application::Instance().GetEventBus()->postpone<EditorEvent::MotionLoaded>({ animation });
                }
            }
        }
        if (ImGui::Button("Reset physics")) {
            m_world->GetPhysicsWorld().Reset();
        }
        if (ImGui::Button("Reload shader")) {
        }
        static bool enablePhysics;
        if (ImGui::Checkbox("Enable physics", &enablePhysics)) {
			m_world->GetPhysicsWorld().SetEnable(enablePhysics);
        }
        ImGui::End();

        m_viewport->OnUIRender();
        m_poseEditor->OnUIRender();
        m_sequencer->OnUIRender();
    }
}
