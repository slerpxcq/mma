#pragma once

#include "Core/Layer/Layer.hpp"

namespace mm
{
	class EditorLayer : public Layer
	{
	public:
		void OnAttach() override ;
		void OnDetach() override ;
		void OnUpdate(float deltaTime) override ;
		void OnUIRender() override ;
		void OnEvent(Event& e) override ;
	};
}


