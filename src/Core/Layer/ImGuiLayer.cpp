#include "mmpch.hpp"
#include "ImGuiLayer.hpp"

#include "Core/Application.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace mm
{
	void ImGuiLayer::OnAttach() 
	{
		m_window = Application::Instance().GetWindow();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		m_font = io.Fonts->AddFontFromFileTTF("resources/fonts/Noto_Sans_JP/static/NotoSansJP-Regular.ttf", FONT_SIZE);
		MM_ASSERT(m_font);

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiLayer::OnDetach() 
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate(float deltaTime) 
	{
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::PushFont(m_font);
	}

	void ImGuiLayer::End()
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
