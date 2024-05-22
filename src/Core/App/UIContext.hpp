#pragma once

#include "Core/Utility/Singleton.hpp"

#include <imgui.h>

namespace mm
{

class UIContext : public Singleton<UIContext>
{
	friend class Singleton<UIContext>;
	static constexpr uint32_t FONT_SIZE = 20;
public:
	void Start();
	void Shutdown();
	void Begin();
	void End();

private:
	GLFWwindow* m_window;
	ImFont* m_font;
};

}

