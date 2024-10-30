#pragma once

#include "ImGuiLayer.hpp"

struct GLFWwindow;

namespace mm
{

class Window;

class ImGuiLayerImpl_GLFW : public ImGuiLayer::Impl
{
public:
	ImGuiLayerImpl_GLFW(const Window& window);
	virtual void Begin() override;
	virtual void End() override;

	~ImGuiLayerImpl_GLFW();

private:
	GLFWwindow* m_window;
};

}

