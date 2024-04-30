#pragma once

#include "Core/App/Layer/Layer.hpp"
#include "Core/App/Event.hpp"

#include "Core/MM/World/World.hpp"

#include "CommandBuffer.hpp"
#include "Viewport.hpp"
#include "PoseEditor.hpp"
#include "Sequencer.hpp"
#include "SceneHierarchy.hpp"
#include "Properties.hpp"
#include "CurveEditor.hpp"

#include <dexode/EventBus.hpp>

namespace mm
{
	class EditorLayer : public Layer
	{
		friend class PoseEditor;
		friend class Viewport;
		friend class Sequencer;
		friend class CurveEditor;
		friend class SceneHierarchy;
		friend class Properties;

	public:
		void OnAttach() override ;
		void OnDetach() override ;
		void OnUpdate(float deltaTime) override ;
		void OnUIRender() override ;

		World& GetWorld() { return *m_world; }
		const Viewport& GetViewport() { return *m_viewport; }

	private:
		std::unique_ptr<World> m_world;

		CommandBuffer m_commandBuffer;

		std::unique_ptr<Viewport> m_viewport;
		std::unique_ptr<PoseEditor> m_poseEditor;
		std::unique_ptr<Sequencer> m_sequencer;
		std::unique_ptr<SceneHierarchy> m_sceneHierarchy;
		std::unique_ptr<Properties> m_properties;
		std::unique_ptr<CurveEditor> m_curveEditor;

		std::unique_ptr<dexode::EventBus::Listener> m_listener;
	};
}


