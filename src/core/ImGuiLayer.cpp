#include "CorePch.hpp"
#include "ImGuiLayer.hpp"

#include "Window/Window.hpp"

#include "ImGuiLayerImpl_GLFW.hpp"

#include <imgui.h>

struct GLFWwindow;

namespace mm
{

ImGuiLayer::ImGuiLayer(const Window& window) 
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImGuiStyle& style = ImGui::GetStyle();
	style.AntiAliasedLines = true;

	ImGui::StyleColorsDark();

	m_font = io.Fonts->AddFontFromFileTTF(Config::FONT_PATH, FONT_SIZE, nullptr, io.Fonts->GetGlyphRangesJapanese());

	switch (GetGraphicsAPI()) {
	case Config::GraphicsAPI::GL4:
		m_impl = MakeScoped<ImGuiLayerImpl_GLFW>(window);
		break;
	default:
		MM_ASSERT("API not supported yet");
		break;
	}
}

void ImGuiLayer::OnUpdate(float deltaTime)
{
}

void ImGuiLayer::OnRender()
{
	Begin();
	ImGui::ShowDemoWindow();
	End();
}

void ImGuiLayer::Begin()
{
	m_impl->Begin();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport();
	ImGui::PushFont(m_font);
}

void ImGuiLayer::End()
{
	ImGui::PopFont();
	m_impl->End();
}

}
