#include "mmpch.hpp"
#include "EditorLayer.hpp"

#include "EditorEvent.hpp"

#include "Core/App/Application.hpp"
#include "Core/App/Core.hpp"
#include "Core/App/Event.hpp"
#include "Core/App/EventBus.hpp"

#include "Core/GL/GLRenderer.hpp"

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
    }

    void EditorLayer::OnUIRender()
    {
        GLRenderer& renderer = GLRenderer::s_instance;
        m_viewport->OnRender(renderer);

        m_viewport->OnUIRender();
        m_poseEditor->OnUIRender();
        m_sequencer->OnUIRender();
        m_sceneHierarchy->OnUIRender();
        m_properties->OnUIRender();
    }
}
