#pragma once

#include "Core/App/Layer/Layer.hpp"
#include "Core/App/Event.hpp"
#include "Core/App/EventUser.hpp"

#include "Core/MM/World.hpp"
#include "Core/GL/GLFrameBuffer.hpp"

#include "Viewport.hpp"
#include "PoseEditor.hpp"

namespace mm
{
	class Model;
	class EditorLayer : public Layer, public EventUser
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
		std::unique_ptr<Viewport> m_viewport;
		std::unique_ptr<PoseEditor> m_poseEditor;
	};
}


