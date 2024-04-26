#include "mmpch.hpp"
#include "EditorLayer.hpp"

#include "EditorEvent.hpp"

#include "Core/App/Application.hpp"

#include "Core/App/Core.hpp"
#include "Core/App/Event.hpp"

#include "Core/GL/GLRenderer.hpp"

#include "Core/App/EventBus.hpp"

#include "Core/MM/Files/VMDFile.hpp"

#include <nfd.h>

namespace mm
{
    void EditorLayer::OnAttach()
    {
        m_listener = std::make_unique<dexode::EventBus::Listener>(EventBus::Instance());

        m_world = std::make_unique<World>();
        m_viewport = std::make_unique<Viewport>(*this);
        m_poseEditor = std::make_unique<PoseEditor>(*this);
        m_sequencer = std::make_unique<Sequencer>(*this);
        m_sceneHierarchy = std::make_unique<SceneHierarchy>(*this);
        m_properties = std::make_unique<Properties>(*this);

        m_sceneHierarchy->SetWorld(m_world.get());
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

    void EditorLayer::LoadModel()
    {
		nfdchar_t* path = nullptr;
		nfdresult_t result = NFD_OpenDialog("pmx", nullptr, &path);
		if (result == NFD_OKAY) {
			Model* model = m_world->LoadModel(path);
			m_model = model;
			EventBus::Instance()->postpone<EditorEvent::ModelLoaded>({ model });
		}
    }

    void EditorLayer::LoadAnimation()
    {
		if (m_model != nullptr) {
			nfdchar_t* path = nullptr;
			nfdresult_t result = NFD_OpenDialog("vmd", nullptr, &path);
			if (result == NFD_OKAY) {
				Animation* animation = m_model->LoadAnimation(path);
				EventBus::Instance()->postpone<EditorEvent::MotionLoaded>({ animation });
				std::free(path);
			}
		}
		else {
			MM_WARN("{0}: Could not load motion: no model is loaded", __FUNCTION__);
		}
    }

    void EditorLayer::ExportAnimation()
    {
		if (m_model != nullptr) {
			nfdchar_t* path = nullptr;
			nfdresult_t result = NFD_SaveDialog("vmd", nullptr, &path);
			if (result == NFD_OKAY) {
				std::unique_ptr<VMDFile> vmd = std::make_unique<VMDFile>(m_model->GetAnim());
				vmd->Serialize(path);
				MM_INFO("{0}: Animation exported successfully", path);
				std::free(path);
			}
		}
		else {
			MM_WARN("{0}: Could not export animation: no model is loaded", __FUNCTION__);
		}
    }

    void EditorLayer::OnUIRender()
    {
        ImGui::Begin("Editor");
        if (ImGui::Button("Load model")) {
            LoadModel();
        }
        if (ImGui::Button("Load animation")) {
            LoadAnimation();
        }
        if (ImGui::Button("Export animation")) {
            ExportAnimation();
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
        m_sceneHierarchy->OnUIRender();
        m_properties->OnUIRender();
    }
}
