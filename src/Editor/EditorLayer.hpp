#pragma once

#include "Core/Layer/Layer.hpp"
#include "Core/Event.hpp"
#include "Core/EventUser.hpp"

#include "Core/MM/World.hpp"

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

	private:
		std::unique_ptr<World> m_scene;
	};
}


