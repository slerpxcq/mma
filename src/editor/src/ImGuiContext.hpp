#pragma once

#include <imgui.h>

struct GLFWwindow;

namespace mm
{

class ImGuiContext
{
	static constexpr uint32_t FONT_SIZE = 20;
	static constexpr char* FONT_PATH = "../../resources/fonts/Noto_Sans_JP/static/NotoSansJP-Regular.ttf";
public:
	ImGuiContext(GLFWwindow* window);
	~ImGuiContext();

	void Begin();
	void End();

private:
	GLFWwindow* m_window;
	ImFont* m_font;
};

}

