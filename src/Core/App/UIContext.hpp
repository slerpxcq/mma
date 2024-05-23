#pragma once

#include "Core/Utility/Singleton.hpp"

#include <imgui.h>

namespace mm
{

class UIContext : public Singleton<UIContext>
{
	friend class Singleton<UIContext>;
	static constexpr uint32_t FONT_SIZE = 20;
	static constexpr char* FONT_PATH = "resources/fonts/Noto_Sans_JP/static/NotoSansJP-Regular.ttf";
public:
	virtual void Startup() override;
	virtual void Shutdown() override;
	void Begin();
	void End();

private:
	GLFWwindow* m_window;
	ImFont* m_font;
};

}

