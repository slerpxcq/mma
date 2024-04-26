#pragma once

#include "Core/App/Layer/Layer.hpp"
#include "Core/App/Event.hpp"

#include "Core/MM/World.hpp"
#include "Core/GL/GLFrameBuffer.hpp"

#include "Viewport.hpp"
#include "PoseEditor.hpp"
#include "Sequencer.hpp"
#include "SceneHierarchy.hpp"

#include "CommandBuffer.hpp"

#include <dexode/EventBus.hpp>

namespace mm
{
	class EditorLayer : public Layer
	{
		friend class PoseEditor;
		friend class Viewport;
		friend class Sequencer;
	public:
		void OnAttach() override ;
		void OnDetach() override ;
		void OnUpdate(float deltaTime) override ;
		void OnUIRender() override ;

		World& GetWorld() { return *m_world; }
		const Viewport& GetViewport() { return *m_viewport; }

	private:
		void LoadModel();
		void LoadAnimation();
		void ExportAnimation();

	private:
		std::unique_ptr<World> m_world;
		Model* m_model;

		CommandBuffer m_commandBuffer;

		std::unique_ptr<Viewport> m_viewport;
		std::unique_ptr<PoseEditor> m_poseEditor;
		std::unique_ptr<Sequencer> m_sequencer;
		std::unique_ptr<SceneHierarchy> m_sceneHierarchy;

		std::unique_ptr<dexode::EventBus::Listener> m_listener;
	};
}


