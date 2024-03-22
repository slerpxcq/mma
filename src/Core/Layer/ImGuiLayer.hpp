#pragma once

#include "Layer.hpp"

struct GLFWwindow;

namespace mm
{
	struct Event;

	class ImGuiLayer : public Layer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;
		void OnEvent(Event& e) override;

		void SetBlockEvent(bool block) {
			m_blockEvent = block;
		}

		void Begin();
		void End();

	private:
		GLFWwindow* m_window;
		bool m_blockEvent;
	};
}


