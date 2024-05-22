#include "mmpch.hpp"
#include "UIContext.hpp"

#include "Application.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace mm
{

void UIContext::Startup()
{
	m_window = Application::Get().m_window;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImGuiStyle& style = ImGui::GetStyle();
	style.AntiAliasedLines = true;

	m_font = io.Fonts->AddFontFromFileTTF("resources/fonts/Noto_Sans_JP/static/NotoSansJP-Regular.ttf", FONT_SIZE, nullptr, io.Fonts->GetGlyphRangesJapanese());
	MM_ASSERT(m_font);

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void UIContext::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UIContext::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport();
	ImGui::PushFont(m_font);
}

void UIContext::End()
{
	ImGui::PopFont();
	ImGuiIO& io = ImGui::GetIO();
	int32_t x, y;
	glfwGetWindowSize(m_window, &x, &y);
	io.DisplaySize = ImVec2(x, y);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}

