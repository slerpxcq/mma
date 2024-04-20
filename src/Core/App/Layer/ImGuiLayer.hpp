#pragma once

#include "Layer.hpp"

struct GLFWwindow;
struct ImFont;

namespace mm
{
	class ImGuiLayer : public ILayer
	{
		static constexpr uint32_t FONT_SIZE = 20;
	public:
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;

		void Begin();
		void End();

		ImFont* GetFont() const { return m_font; }

	private:
		GLFWwindow* m_window;
		ImFont* m_font;
	};
}


