#pragma once

#include "Core/App/Layer/Layer.hpp"
#include "Core/App/Event.hpp"

#include "Core/MM/World.hpp"
#include "Core/GL/GLFrameBuffer.hpp"

#include "Viewport.hpp"
#include "PoseEditor.hpp"
#include "KeyframeEditor.hpp"

#include <dexode/EventBus.hpp>

namespace mm
{
	class EditorLayer : public Layer
	{
	public:
		void OnAttach() override ;
		void OnDetach() override ;
		void OnUpdate(float deltaTime) override ;
		void OnUIRender() override ;

		World& GetWorld() { return *m_world; }
		const Viewport& GetViewport() { return *m_viewport; }

	private:
		std::unique_ptr<World> m_world;
		Model* m_model;

		std::unique_ptr<Viewport> m_viewport;
		std::unique_ptr<PoseEditor> m_poseEditor;
		std::unique_ptr<KeyframeEditor> m_keyframeEditor;

		std::unique_ptr<dexode::EventBus::Listener> m_listener;
	};
}


