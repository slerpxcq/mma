#include "CorePch.hpp"
#include "ImGuiLayerImpl_GLFW.hpp"

#include "Window/Window.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace mm
{

ImGuiLayerImpl_GLFW::ImGuiLayerImpl_GLFW(const Window& window) :
	m_window{ static_cast<GLFWwindow*>(window.GetHandle()) }
{
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGuiLayerImpl_GLFW::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
}

void ImGuiLayerImpl_GLFW::End()
{
	ImGuiIO& io = ImGui::GetIO();
	int32_t x, y;
	glfwGetWindowSize(m_window, &x, &y);
	io.DisplaySize = ImVec2(x, y);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

ImGuiLayerImpl_GLFW::~ImGuiLayerImpl_GLFW()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

}
