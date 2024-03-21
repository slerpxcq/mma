#pragma once

#include "../AppLayer.hpp"

struct GLFWwindow;

namespace mm
{
	struct Event;

	class ImGuiLayer : public AppLayer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;
		void OnEvent(Event& e) override;

		void Begin();
		void End();

	private:
		GLFWwindow* m_window;
	};
}


